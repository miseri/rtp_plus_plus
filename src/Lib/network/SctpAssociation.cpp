#include "CorePch.h"
#include <rtp++/network/SctpAssociation.h>
#include <rtp++/RtpTime.h>
#include <rtp++/network/NetworkPacket.h>
#include <rtp++/network/Sctp.h>
#include <rtp++/application/ApplicationContext.h>

namespace rtp_plus_plus
{
namespace sctp
{

#ifdef ENABLE_SCTP_USERLAND

int receive_cb(struct socket *sock, union sctp_sockstore addr, void *data,
           size_t datalen, struct sctp_rcvinfo rcv, int flags, void *ulp_info)
{
  peer_connection *pc;
  pc = (peer_connection*)ulp_info;

  if (data) {
    pc->lock_peer_connection();
    if (flags & MSG_NOTIFICATION) {
      pc->handle_notification(sock, (union sctp_notification *)data, datalen);
    } else {
      pc->handle_message((char*)data, datalen, ntohl(rcv.rcv_ppid), rcv.rcv_sid);
    }
    pc->unlock_peer_connection();
  } else {
    usrsctp_close(sock);
  }
  return (1);
}

peer_connection::peer_connection(uint16_t uiSctpPort,
                                 uint16_t uiLocalUdpEncapsPort,
                                 uint16_t uiRemoteUdpEncapsPort,
                                 bool bActivateSctp)
  :m_bIsConnected(false),
    m_uiSctpPort(uiSctpPort)
#if 0
   ,m_uiLocalUdpEncapsPort(uiLocalUdpEncapsPort),
    m_uiRemoteUdpEncapsPort(uiRemoteUdpEncapsPort)
  #endif
{
  // HACK to not start SCTP in RtpPacketiser application
  if (bActivateSctp)
  {
    uint32_t i;
    struct channel *channel;

    for (i = 0; i < NUMBER_OF_CHANNELS; i++) {
      channel = &(m_channels[i]);
      channel->id = i;
      channel->state = DATA_CHANNEL_CLOSED;
      // channel->pr_policy = SCTP_PR_SCTP_NONE;
      channel->pr_policy = SCTP_PR_POLICY_SCTP_NONE;
      channel->pr_value = 0;
      channel->i_stream = 0;
      channel->o_stream = 0;
      channel->unordered = false;
    }
    for (i = 0; i < NUMBER_OF_STREAMS; i++) {
      m_i_stream_channel[i] = NULL;
      m_o_stream_channel[i] = NULL;
      m_o_stream_buffer[i] = 0;
    }
    m_o_stream_buffer_counter = 0;
    m_sock = NULL;

#if defined(__Userspace_os_Windows)
    InitializeCriticalSection(&(m_mutex));
#else
    pthread_mutex_init(&m_mutex, NULL);
#endif

#if 0
    //  struct socket *sock;
    struct sockaddr_in addr;
    socklen_t addr_len;
    char line[LINE_LENGTH + 1];
    unsigned int unordered, policy, value, id, seconds;
#endif

    //unsigned int i;
    //struct channel *channel;
    const int on = 1;
    struct sctp_assoc_value av;
    struct sctp_event event;
    struct sctp_udpencaps encaps;
    uint16_t event_types[] = {SCTP_ASSOC_CHANGE,
                              SCTP_PEER_ADDR_CHANGE,
                              SCTP_REMOTE_ERROR,
                              SCTP_SHUTDOWN_EVENT,
                              SCTP_ADAPTATION_INDICATION,
                              SCTP_SEND_FAILED_EVENT,
                              SCTP_STREAM_RESET_EVENT,
                              SCTP_STREAM_CHANGE_EVENT};

    // let application context handle this so that it can shut down cleanly
    // usrsctp_init(uiLocalUdpEncapsPort);
    app::ApplicationContext::initialiseUsrSctpPort(uiLocalUdpEncapsPort);

    usrsctp_sysctl_set_sctp_debug_on(0);
    usrsctp_sysctl_set_sctp_blackhole(2);

    if ((m_sock = usrsctp_socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP, receive_cb, NULL, 0, this /* pass handle to this*/)) == NULL) {
      perror("socket");
      LOG(WARNING) << "Unable to get SCTP socket";
    }

    // update send buffer size
    socklen_t opt_len = (socklen_t)sizeof(int);
    int cur_buf_size = 0;
    if (usrsctp_getsockopt(m_sock, SOL_SOCKET, SO_SNDBUF, &cur_buf_size, &opt_len) < 0) {
      perror("usrsctp_getsockopt");
    }
    VLOG(2) << "Change send socket buffer size from " << cur_buf_size;
    // int snd_buf_size = 1<<20; /* 1 MB */
    int snd_buf_size = 1<<22; /* 4 MB */
    if (usrsctp_setsockopt(m_sock, SOL_SOCKET, SO_SNDBUF, &snd_buf_size, sizeof(int)) < 0) {
      perror("usrsctp_setsockopt");
    }
    else
    {
      if (usrsctp_getsockopt(m_sock, SOL_SOCKET, SO_SNDBUF, &cur_buf_size, &opt_len) < 0) {
        perror("usrsctp_getsockopt");
      }
      else
      {
        VLOG(2) << "Updated send socket buffer size from " << cur_buf_size;
      }
    }

    // update receive buffer size
    opt_len = (socklen_t)sizeof(int);
    cur_buf_size = 0;
    if (usrsctp_getsockopt(m_sock, SOL_SOCKET, SO_RCVBUF, &cur_buf_size, &opt_len) < 0) {
      perror("usrsctp_getsockopt");
    }
    VLOG(2) << "Change recv socket buffer size from " << cur_buf_size;
    // snd_buf_size = 1<<20; /* 1 MB */
    snd_buf_size = 1<<22; /* 4 MB */
    if (usrsctp_setsockopt(m_sock, SOL_SOCKET, SO_RCVBUF, &snd_buf_size, sizeof(int)) < 0) {
      perror("usrsctp_setsockopt");
    }
    else
    {
      if (usrsctp_getsockopt(m_sock, SOL_SOCKET, SO_RCVBUF, &cur_buf_size, &opt_len) < 0) {
        perror("usrsctp_getsockopt");
      }
      else
      {
        VLOG(2) << "Updated recv socket buffer size to " << cur_buf_size;
      }
    }

    // UDP ENCAPS: using same port
    memset(&encaps, 0, sizeof(struct sctp_udpencaps));
    encaps.sue_address.ss_family = AF_INET6;
    encaps.sue_port = htons(uiRemoteUdpEncapsPort);
    if (usrsctp_setsockopt(m_sock, IPPROTO_SCTP, SCTP_REMOTE_UDP_ENCAPS_PORT, (const void*)&encaps, (socklen_t)sizeof(struct sctp_udpencaps)) < 0) {
      perror("setsockopt");
    }

    if (usrsctp_setsockopt(m_sock, IPPROTO_SCTP, SCTP_RECVRCVINFO, &on, sizeof(int)) < 0) {
      perror("setsockopt SCTP_RECVRCVINFO");
    }
    if (usrsctp_setsockopt(m_sock, IPPROTO_SCTP, SCTP_EXPLICIT_EOR, &on, sizeof(int)) < 0) {
      perror("setsockopt SCTP_EXPLICIT_EOR");
    }
    /* Allow resetting streams. */
    av.assoc_id = SCTP_ALL_ASSOC;
    av.assoc_value = SCTP_ENABLE_RESET_STREAM_REQ | SCTP_ENABLE_CHANGE_ASSOC_REQ;
    if (usrsctp_setsockopt(m_sock, IPPROTO_SCTP, SCTP_ENABLE_STREAM_RESET, &av, sizeof(struct sctp_assoc_value)) < 0) {
      perror("setsockopt SCTP_ENABLE_STREAM_RESET");
    }
    /* Enable the events of interest. */
    memset(&event, 0, sizeof(event));
    event.se_assoc_id = SCTP_ALL_ASSOC;
    event.se_on = 1;
    for (i = 0; i < sizeof(event_types)/sizeof(uint16_t); i++) {
      event.se_type = event_types[i];
      if (usrsctp_setsockopt(m_sock, IPPROTO_SCTP, SCTP_EVENT, &event, sizeof(event)) < 0) {
        perror("setsockopt SCTP_EVENT");
      }
      else
      {
        VLOG(2) << "SCTP event notification enabled for " << event_types[i];
      }
    }

#if 1
    #define SCTP_PLUGGABLE_SS		0x00001203
    struct sctp_assoc_value stream_scheduler;
    stream_scheduler.assoc_id = SCTP_ALL_ASSOC;
    stream_scheduler.assoc_value = SCTP_SS_PRIORITY;
    if (usrsctp_setsockopt(m_sock, IPPROTO_SCTP, SCTP_PLUGGABLE_SS, &stream_scheduler, sizeof(stream_scheduler)) < 0) {
      perror("setsockopt Failed to set plugable stream scheduler");
    }
#endif
  }
}

bool peer_connection::setChannelPriority(uint16_t uiChannel, uint16_t uiPriority)
{
  struct sctp_stream_value {
       sctp_assoc_t assoc_id;
       uint16_t stream_id;
       uint16_t stream_value;
     };
#define SCTP_SS_VALUE 0x00001204

  struct sctp_stream_value sctp_prio;
  sctp_prio.assoc_id = SCTP_ALL_ASSOC;
  sctp_prio.stream_id = uiChannel;
  sctp_prio.stream_value = uiPriority;

  if (usrsctp_setsockopt(m_sock, IPPROTO_SCTP, SCTP_SS_VALUE, &sctp_prio, sizeof(sctp_prio)) < 0) {
    perror("setsockopt Failed to set priority of channel");
    return false;
  }
  else
  {
    return true;
  }
}

void peer_connection::lock_peer_connection()
{
#if defined(__Userspace_os_Windows)
  EnterCriticalSection(&(m_mutex));
#else
  pthread_mutex_lock(&m_mutex);
#endif
}

void peer_connection::unlock_peer_connection()
{
#if defined(__Userspace_os_Windows)
  LeaveCriticalSection(&(m_mutex));
#else
  pthread_mutex_unlock(&m_mutex);
#endif
}

channel* peer_connection::find_channel_by_i_stream(uint16_t i_stream)
{
  if (i_stream < NUMBER_OF_STREAMS) {
    return (m_i_stream_channel[i_stream]);
  } else {
    return (NULL);
  }
}

channel* peer_connection::find_channel_by_o_stream(uint16_t o_stream)
{
  if (o_stream < NUMBER_OF_STREAMS) {
    return (m_o_stream_channel[o_stream]);
  } else {
    return (NULL);
  }
}

channel* peer_connection::find_free_channel()
{
  uint32_t i;

  for (i = 0; i < NUMBER_OF_CHANNELS; i++) {
    if (m_channels[i].state == DATA_CHANNEL_CLOSED) {
      break;
    }
  }
  if (i == NUMBER_OF_CHANNELS) {
    return (NULL);
  } else {
    return (&(m_channels[i]));
  }
}

uint16_t peer_connection::find_free_o_stream()
{
  struct sctp_status status;
  uint32_t i, limit;
  socklen_t len;

  len = (socklen_t)sizeof(struct sctp_status);
  if (usrsctp_getsockopt(m_sock, IPPROTO_SCTP, SCTP_STATUS, &status, &len) < 0) {
    perror("getsockopt");
    return (0);
  }
  if (status.sstat_outstrms < NUMBER_OF_STREAMS) {
    limit = status.sstat_outstrms;
  } else {
    limit = NUMBER_OF_STREAMS;
  }
  /* stream id 0 is reserved */
  for (i = 1; i < limit; i++) {
    if (m_o_stream_channel[i] == NULL) {
      break;
    }
  }
  if (i == limit) {
    return (0);
  } else {
    return ((uint16_t)i);
  }
}

void peer_connection::request_more_o_streams()
{
  struct sctp_status status;
  struct sctp_add_streams sas;
  uint32_t i, o_streams_needed;
  socklen_t len;

  o_streams_needed = 0;
  for (i = 0; i < NUMBER_OF_CHANNELS; i++) {
    if ((m_channels[i].state == DATA_CHANNEL_CONNECTING) &&
        (m_channels[i].o_stream == 0)) {
      o_streams_needed++;
    }
  }
  len = (socklen_t)sizeof(struct sctp_status);
  if (usrsctp_getsockopt(m_sock, IPPROTO_SCTP, SCTP_STATUS, &status, &len) < 0) {
    perror("getsockopt");
    return;
  }
  if (status.sstat_outstrms + o_streams_needed > NUMBER_OF_STREAMS) {
    o_streams_needed = NUMBER_OF_STREAMS - status.sstat_outstrms;
  }
  if (o_streams_needed == 0) {
    return;
  }
  memset(&sas, 0, sizeof(struct sctp_add_streams));
  sas.sas_instrms = 0;
  sas.sas_outstrms = (uint16_t)o_streams_needed; /* XXX eror handling */
  if (usrsctp_setsockopt(m_sock, IPPROTO_SCTP, SCTP_ADD_STREAMS, &sas, (socklen_t)sizeof(struct sctp_add_streams)) < 0) {
    perror("setsockopt");
  }
  return;
}

channel* peer_connection::open_channel(bool bUnordered, PrPolicy ePrPolicy, uint32_t pr_value)
{
  // TODO: RAII
  lock_peer_connection();

  struct channel *channel;
  uint16_t o_stream;

  switch (ePrPolicy)
  {
    case SCTP_PR_POLICY_SCTP_BUF:
    {
      unlock_peer_connection();
      return (NULL);
    }
    case SCTP_PR_POLICY_SCTP_NONE:
    {
      if (pr_value != 0)
      {
        unlock_peer_connection();
        return (NULL);
      }
    }
    case SCTP_PR_POLICY_SCTP_TTL:
    case SCTP_PR_POLICY_SCTP_RTX:
    default:
    {
      // NOOP
      break;
    }
  }

  if ((channel = find_free_channel()) == NULL) {
    unlock_peer_connection();
    return (NULL);
  }

  o_stream = find_free_o_stream();
  if ((o_stream == 0) ||
      (send_open_request_message(m_sock, o_stream, bUnordered, ePrPolicy, pr_value))) {
    channel->state = DATA_CHANNEL_CONNECTING;
    channel->unordered = bUnordered;
    channel->pr_policy = ePrPolicy;
    channel->pr_value = pr_value;
    channel->o_stream = o_stream;
    if (o_stream != 0) {
      m_o_stream_channel[o_stream] = channel;
    } else {
      request_more_o_streams();
    }
    unlock_peer_connection();
    return (channel);
  } else {
    unlock_peer_connection();
    return (NULL);
  }
}

int peer_connection::send_user_message(size_t id, char *message, size_t length)
{
  if (id >= NUMBER_OF_CHANNELS)
  {
    return 0;
  }

  channel *c = &m_channels[id];
  struct sctp_sendv_spa spa;

  if (c == NULL) {
    return (0);
  }
  if ((c->state != DATA_CHANNEL_OPEN) &&
      (c->state != DATA_CHANNEL_CONNECTING)) {
    /* XXX: What to do in other states */
    return (0);
  }

  memset(&spa, 0, sizeof(struct sctp_sendv_spa));
  spa.sendv_sndinfo.snd_sid = c->o_stream;
  if ((c->state == DATA_CHANNEL_OPEN) &&
      (c->unordered)) {
    spa.sendv_sndinfo.snd_flags = SCTP_EOR | SCTP_UNORDERED;
  } else {
    spa.sendv_sndinfo.snd_flags = SCTP_EOR;
  }
  spa.sendv_sndinfo.snd_ppid = htonl(DATA_CHANNEL_PPID_DOMSTRING);
  spa.sendv_flags = SCTP_SEND_SNDINFO_VALID;
  if ((c->pr_policy == SCTP_PR_SCTP_TTL) ||
      (c->pr_policy == SCTP_PR_SCTP_RTX)) {
    spa.sendv_prinfo.pr_policy = c->pr_policy;
    spa.sendv_prinfo.pr_value = c->pr_value;
    spa.sendv_flags |= SCTP_SEND_PRINFO_VALID;
  }

  lock_peer_connection();
  if (usrsctp_sendv(m_sock,
                    message, length,
                    NULL, 0,
                    &spa, (socklen_t)sizeof(struct sctp_sendv_spa),
                    SCTP_SENDV_SPA, 0) < 0)
  {
    perror("sctp_sendv");
    unlock_peer_connection();
    return (0);
  }
  else
  {
    unlock_peer_connection();
    return (1);
  }
}

void peer_connection::reset_outgoing_stream(uint16_t o_stream)
{
  uint32_t i;

  for (i = 0; i < m_o_stream_buffer_counter; i++) {
    if (m_o_stream_buffer[i] == o_stream) {
      return;
    }
  }
  m_o_stream_buffer[m_o_stream_buffer_counter++] = o_stream;
  return;
}

void peer_connection::send_outgoing_stream_reset()
{
  struct sctp_reset_streams *srs;
  uint32_t i;
  size_t len;

  if (m_o_stream_buffer_counter == 0) {
    return;
  }
  len = sizeof(sctp_assoc_t) + (2 + m_o_stream_buffer_counter) * sizeof(uint16_t);
  srs = (struct sctp_reset_streams *)malloc(len);
  if (srs == NULL) {
    return;
  }
  memset(srs, 0, len);
  srs->srs_flags = SCTP_STREAM_RESET_OUTGOING;
  srs->srs_number_streams = m_o_stream_buffer_counter;
  for (i = 0; i < m_o_stream_buffer_counter; i++) {
    srs->srs_stream_list[i] = m_o_stream_buffer[i];
  }
  if (usrsctp_setsockopt(m_sock, IPPROTO_SCTP, SCTP_RESET_STREAMS, srs, (socklen_t)len) < 0) {
    perror("setsockopt");
  } else {
    for (i = 0; i < m_o_stream_buffer_counter; i++) {
      srs->srs_stream_list[i] = 0;
    }
    m_o_stream_buffer_counter = 0;
  }
  free(srs);
  return;
}

void peer_connection::close_channel(uint32_t id)
{
  if (id >= NUMBER_OF_CHANNELS) return;

  channel *c = &m_channels[id];

  if (c->state != DATA_CHANNEL_OPEN) return;

  lock_peer_connection();
  reset_outgoing_stream(c->o_stream);
  send_outgoing_stream_reset();
  c->state = DATA_CHANNEL_CLOSING;
  unlock_peer_connection();

  return;
}

void peer_connection::handle_open_request_message(struct rtcweb_datachannel_open_request *req,
                            size_t length,
                            uint16_t i_stream)
{
  channel *c;
  uint32_t pr_value;
  PrPolicy pr_policy;
  uint16_t o_stream;
  uint8_t unordered;

  if ((c = find_channel_by_i_stream(i_stream))) {
    printf("Hmm, channel %d is in state %d instead of CLOSED.\n",
           c->id, c->state);
    return;
    /* XXX: some error handling */
  }
  if ((c = find_free_channel()) == NULL) {
    /* XXX: some error handling */
    return;
  }
  switch (req->channel_type) {
  case DATA_CHANNEL_RELIABLE:
//    pr_policy = SCTP_PR_SCTP_NONE;
    pr_policy = SCTP_PR_POLICY_SCTP_NONE;
    break;
  /* XXX Doesn't make sense */
  case DATA_CHANNEL_RELIABLE_STREAM:
//    pr_policy = SCTP_PR_SCTP_NONE;
    pr_policy = SCTP_PR_POLICY_SCTP_NONE;
    break;
  /* XXX Doesn't make sense */
  case DATA_CHANNEL_UNRELIABLE:
//    pr_policy = SCTP_PR_SCTP_TTL;
    pr_policy = SCTP_PR_POLICY_SCTP_TTL;
    break;
  case DATA_CHANNEL_PARTIAL_RELIABLE_REXMIT:
//    pr_policy = SCTP_PR_SCTP_RTX;
    pr_policy = SCTP_PR_POLICY_SCTP_RTX;
    break;
  case DATA_CHANNEL_PARTIAL_RELIABLE_TIMED:
//    pr_policy = SCTP_PR_SCTP_TTL;
    pr_policy = SCTP_PR_POLICY_SCTP_TTL;
    break;
  default:
    /* XXX error handling */
    break;
  }
  pr_value = ntohs(req->reliability_params);
  if (ntohs(req->flags) & DATA_CHANNEL_FLAG_OUT_OF_ORDER_ALLOWED) {
    unordered = 1;
  } else {
    unordered = 0;
  }
  o_stream = find_free_o_stream();
  if ((o_stream == 0) || send_open_response_message(m_sock, o_stream, i_stream)) {
    c->state = DATA_CHANNEL_CONNECTING;
    c->unordered = unordered;
    c->pr_policy = pr_policy;
    c->pr_value = pr_value;
    c->i_stream = i_stream;
    m_i_stream_channel[i_stream] = c;
    if (o_stream != 0) {
      c->o_stream = o_stream;
      m_o_stream_channel[o_stream] = c;
    } else {
      request_more_o_streams();
    }
  } else {
    /* error handling */
  }
  return;
}

void peer_connection::handle_open_response_message(                             struct rtcweb_datachannel_open_response *rsp,
                             size_t length, uint16_t i_stream)
{
  uint16_t o_stream;
  struct channel *channel;

  o_stream = ntohs(rsp->reverse_stream);
  channel = find_channel_by_o_stream( o_stream);
  if (channel == NULL) {
    /* XXX: some error handling */
  }
  if (channel->state != DATA_CHANNEL_CONNECTING) {
    /* XXX: some error handling */
  }
  if (find_channel_by_i_stream(i_stream)) {
    /* XXX: some error handling */
  }
  channel->i_stream = i_stream;
  channel->state = DATA_CHANNEL_OPEN;
  m_i_stream_channel[i_stream] = channel;
  send_open_ack_message(m_sock, o_stream);
  return;
}

void peer_connection::handle_open_ack_message(                        struct rtcweb_datachannel_ack *ack,
                        size_t length, uint16_t i_stream)
{
  struct channel *channel;

  channel = find_channel_by_i_stream(i_stream);
  if (channel == NULL) {
    /* XXX: some error handling */
  }
  if (channel->state == DATA_CHANNEL_OPEN) {
    return;
  }
  if (channel->state != DATA_CHANNEL_CONNECTING) {
    /* XXX: error handling */
    return;
  }
  channel->state = DATA_CHANNEL_OPEN;
  return;
}

void peer_connection::handle_unknown_message(char *msg, size_t length, uint16_t i_stream)
{
  /* XXX: Send an error message */
  return;
}

void peer_connection::handle_data_message(char *buffer, size_t length, uint16_t i_stream)
{
  struct channel *channel;

  channel = find_channel_by_i_stream(i_stream);
  if (channel == NULL) {
    /* XXX: Some error handling */
    return;
  }
  if (channel->state == DATA_CHANNEL_CONNECTING) {
    /* Implicit ACK */
    channel->state = DATA_CHANNEL_OPEN;
  }
  if (channel->state != DATA_CHANNEL_OPEN) {
    /* XXX: What about other states? */
    /* XXX: Some error handling */
    return;
  } else {
    /* Assuming DATA_CHANNEL_PPID_DOMSTRING */
    /* XXX: Protect for non 0 terminated buffer */
//    printf("Message received of length %lu on channel with id %d: %.*s\n",
//           length, channel->id, (int)length, buffer);
    NetworkPacket networkPacket(RtpTime::getNTPTimeStamp());
    uint8_t* pData = new uint8_t[length];
    memcpy(pData, buffer, length);
    networkPacket.setData(pData, length);

    if (m_onSctpRecv)
      m_onSctpRecv(channel->id, networkPacket);
    else
    {
      LOG_FIRST_N(WARNING, 1) << "No SCTP receive callback configured";
    }
  }
  return;
}

void peer_connection::handle_message(char *buffer, size_t length, uint32_t ppid, uint16_t i_stream)
{
  struct rtcweb_datachannel_open_request *req;
  struct rtcweb_datachannel_open_response *rsp;
  struct rtcweb_datachannel_ack *ack, *msg;

  switch (ppid) {
  case DATA_CHANNEL_PPID_CONTROL:
    if (length < sizeof(struct rtcweb_datachannel_ack)) {
      return;
    }
    msg = (struct rtcweb_datachannel_ack *)buffer;
    switch (msg->msg_type) {
    case DATA_CHANNEL_OPEN_REQUEST:
      if (length < sizeof(struct rtcweb_datachannel_open_request)) {
        /* XXX: error handling? */
        return;
      }
      req = (struct rtcweb_datachannel_open_request *)buffer;
      handle_open_request_message(req, length, i_stream);
      break;
    case DATA_CHANNEL_OPEN_RESPONSE:
      if (length < sizeof(struct rtcweb_datachannel_open_response)) {
        /* XXX: error handling? */
        return;
      }
      rsp = (struct rtcweb_datachannel_open_response *)buffer;
      handle_open_response_message(rsp, length, i_stream);
      break;
    case DATA_CHANNEL_ACK:
      if (length < sizeof(struct rtcweb_datachannel_ack)) {
        /* XXX: error handling? */
        return;
      }
      ack = (struct rtcweb_datachannel_ack *)buffer;
      handle_open_ack_message(ack, length, i_stream);
      break;
    default:
      handle_unknown_message(buffer, length, i_stream);
      break;
    }
    break;
  case DATA_CHANNEL_PPID_DOMSTRING:
  case DATA_CHANNEL_PPID_BINARY:
    handle_data_message(buffer, length, i_stream);
    break;
  default:
    printf("Message of length %u, PPID %u on stream %u received.\n",
           length, ppid, i_stream);
    break;
  }
}

void peer_connection::handle_association_change_event(struct socket *sock, struct sctp_assoc_change *sac)
{
  unsigned int i, n;

  std::ostringstream ostr;
  ostr << "Association change ";
  switch (sac->sac_state) {
  case SCTP_COMM_UP:
    ostr << "SCTP_COMM_UP";
    break;
  case SCTP_COMM_LOST:
    ostr << "SCTP_COMM_LOST";
    break;
  case SCTP_RESTART:
    ostr << "SCTP_RESTART";
    break;
  case SCTP_SHUTDOWN_COMP:
    ostr << "SCTP_SHUTDOWN_COMP";
    break;
  case SCTP_CANT_STR_ASSOC:
    ostr << "SCTP_CANT_STR_ASSOC";
    break;
  default:
    ostr << "UNKNOWN";
    break;
  }
  ostr << ", streams (in/out) = (" << sac->sac_inbound_streams << "/" << sac->sac_outbound_streams << ")",

  n = sac->sac_length - sizeof(struct sctp_assoc_change);
  if (((sac->sac_state == SCTP_COMM_UP) ||
       (sac->sac_state == SCTP_RESTART)) && (n > 0)) {
    ostr << ", supports";
    for (i = 0; i < n; i++) {
      switch (sac->sac_info[i]) {
      case SCTP_ASSOC_SUPPORTS_PR:
        ostr << " PR";
        break;
      case SCTP_ASSOC_SUPPORTS_AUTH:
        ostr << " AUTH";
        break;
      case SCTP_ASSOC_SUPPORTS_ASCONF:
        ostr << " ASCONF";
        break;
      case SCTP_ASSOC_SUPPORTS_MULTIBUF:
        ostr << " MULTIBUF";
        break;
      case SCTP_ASSOC_SUPPORTS_RE_CONFIG:
        ostr << " RE-CONFIG";
        break;
      default:
        ostr << " UNKNOWN(0x" <<  sac->sac_info[i] << ")";
        break;
      }
    }
  } else if (((sac->sac_state == SCTP_COMM_LOST) ||
              (sac->sac_state == SCTP_CANT_STR_ASSOC)) && (n > 0)) {
    ostr << ", ABORT =";
    for (i = 0; i < n; i++) {
      ostr << " 0x" << sac->sac_info[i];
    }
  }
  VLOG(2) << ostr.str();

  if ((sac->sac_state == SCTP_COMM_UP) && (m_sock == NULL)) {
    m_sock = sock;
  }
  return;
}

void peer_connection::handle_peer_address_change_event(struct sctp_paddr_change *spc)
{
  char addr_buf[INET6_ADDRSTRLEN];
  const char *addr;
  struct sockaddr_in *sin;
  struct sockaddr_in6 *sin6;

  std::ostringstream ostr;
  switch (spc->spc_aaddr.ss_family) {
    case AF_INET:
    {
      sin = (struct sockaddr_in *)&spc->spc_aaddr;
      addr = inet_ntop(AF_INET, &sin->sin_addr, addr_buf, INET6_ADDRSTRLEN);
      ostr << "Peer address " << addr<< " is now ";
      break;
    }
    case AF_INET6:
    {
      sin6 = (struct sockaddr_in6 *)&spc->spc_aaddr;
      addr = inet_ntop(AF_INET6, &sin6->sin6_addr, addr_buf, INET6_ADDRSTRLEN);
      ostr << "Peer address " << addr<< " is now ";
      break;
    }
    default:
      break;
  }
  switch (spc->spc_state)
  {
  case SCTP_ADDR_AVAILABLE:
    ostr << "SCTP_ADDR_AVAILABLE";
    break;
  case SCTP_ADDR_UNREACHABLE:
    ostr << "SCTP_ADDR_UNREACHABLE";
    break;
  case SCTP_ADDR_REMOVED:
    ostr << "SCTP_ADDR_REMOVED";
    break;
  case SCTP_ADDR_ADDED:
    ostr << "SCTP_ADDR_ADDED";
    break;
  case SCTP_ADDR_MADE_PRIM:
    ostr << "SCTP_ADDR_MADE_PRIM";
    break;
  case SCTP_ADDR_CONFIRMED:
    ostr << "SCTP_ADDR_CONFIRMED";
    break;
  default:
    ostr << "UNKNOWN";
    break;
  }
  ostr << " (error = 0x" << spc->spc_error << ")";
  VLOG(2) << ostr.str();
  return;
}

void peer_connection::handle_adaptation_indication(struct sctp_adaptation_event *sai)
{
  VLOG(2) << "Adaptation indication: " <<  sai-> sai_adaptation_ind;
  return;
}

void peer_connection::handle_shutdown_event(struct sctp_shutdown_event *sse)
{
  VLOG(2) << "Shutdown event.";
  /* XXX: notify all channels. */
  return;
}

void peer_connection::handle_stream_reset_event(struct sctp_stream_reset_event *strrst)
{
  uint32_t n, i;
  struct channel *channel;

  if (!(strrst->strreset_flags & SCTP_STREAM_RESET_DENIED) &&
      !(strrst->strreset_flags & SCTP_STREAM_RESET_FAILED)) {
    n = (strrst->strreset_length - sizeof(struct sctp_stream_reset_event)) / sizeof(uint16_t);
    for (i = 0; i < n; i++) {
      if (strrst->strreset_flags & SCTP_STREAM_RESET_INCOMING_SSN) {
        channel = find_channel_by_i_stream(strrst->strreset_stream_list[i]);
        if (channel != NULL) {
          m_i_stream_channel[channel->i_stream] = NULL;
          channel->i_stream = 0;
          if (channel->o_stream == 0) {
//            channel->pr_policy = SCTP_PR_SCTP_NONE;
            channel->pr_policy = SCTP_PR_POLICY_SCTP_NONE;
            channel->pr_value = 0;
            channel->unordered = 0;
            channel->state = DATA_CHANNEL_CLOSED;
          } else {
            reset_outgoing_stream(channel->o_stream);
            channel->state = DATA_CHANNEL_CLOSING;
          }
        }
      }
      if (strrst->strreset_flags & SCTP_STREAM_RESET_OUTGOING_SSN) {
        channel = find_channel_by_o_stream(strrst->strreset_stream_list[i]);
        if (channel != NULL) {
          m_o_stream_channel[channel->o_stream] = NULL;
          channel->o_stream = 0;
          if (channel->i_stream == 0) {
            channel->pr_policy = SCTP_PR_POLICY_SCTP_NONE;
            channel->pr_value = 0;
            channel->unordered = false;
            channel->state = DATA_CHANNEL_CLOSED;
          }
        }
      }
    }
  }
  return;
}

void peer_connection::handle_stream_change_event(struct sctp_stream_change_event *strchg)
{
  uint16_t o_stream;
  uint32_t i;
  struct channel *channel;

  for (i = 0; i < NUMBER_OF_CHANNELS; i++) {
    channel = &(m_channels[i]);
    if ((channel->state == DATA_CHANNEL_CONNECTING) &&
        (channel->o_stream == 0)) {
      if ((strchg->strchange_flags & SCTP_STREAM_CHANGE_DENIED) ||
          (strchg->strchange_flags & SCTP_STREAM_CHANGE_FAILED)) {
        channel->state = DATA_CHANNEL_CLOSED;
        channel->unordered = false;
        channel->pr_policy = SCTP_PR_POLICY_SCTP_NONE;
        channel->pr_value = 0;
        channel->o_stream = 0;
      } else {
        o_stream = find_free_o_stream();
        if (o_stream != 0) {
          if (channel->i_stream != 0) {
            if (send_open_response_message(m_sock, o_stream, channel->i_stream)) {
              channel->o_stream = o_stream;
              m_o_stream_channel[o_stream] = channel;
            } else {
              /* XXX: error handling */
            }
          } else {
            if (send_open_request_message(m_sock, o_stream, channel->unordered, channel->pr_policy, channel->pr_value)) {
              channel->o_stream = o_stream;
              m_o_stream_channel[o_stream] = channel;
            } else {
              channel->state = DATA_CHANNEL_CLOSED;
              channel->unordered = false;
              channel->pr_policy = SCTP_PR_POLICY_SCTP_NONE;
              channel->pr_value = 0;
              channel->o_stream = 0;
            }
          }
        } else {
          break;
        }
      }
    }
  }
  return;
}

void peer_connection::handle_remote_error_event(struct sctp_remote_error *sre)
{
  VLOG(2) << "Remote Error (error = 0x" << sre->sre_error;

#ifdef PRINT_OUT_REMOTE_DATA
  size_t n = sre->sre_length - sizeof(struct sctp_remote_error);
  printf("Remote Error (error = 0x%04x): ", );
  for (size_t i = 0; i < n; i++) {
    printf(" 0x%02x", sre-> sre_data[i]);
  }
  printf(".\n");
#endif
  return;
}

void peer_connection::handle_send_failed_event(struct sctp_send_failed_event *ssfe)
{
  std::ostringstream ostr;
  if (ssfe->ssfe_flags & SCTP_DATA_UNSENT) {
    ostr << "Unsent ";
  }
  if (ssfe->ssfe_flags & SCTP_DATA_SENT) {
    ostr << "Sent ";
  }
  if (ssfe->ssfe_flags & ~(SCTP_DATA_SENT | SCTP_DATA_UNSENT)) {
    ostr << "(flags = " << ssfe->ssfe_flags << ") ";
  }

  size_t n = ssfe->ssfe_length - sizeof(struct sctp_send_failed_event);

  ostr << "message (Size: " << n << ") with PPID = "
       << ntohl(ssfe->ssfe_info.snd_ppid)
       << ", SID = " << ssfe->ssfe_info.snd_flags << ", flags: 0x"
       << ssfe->ssfe_info.snd_sid << " due to error = 0x" <<  ssfe->ssfe_error;

  VLOG(2) << ostr.str();

#if 0
  for (size_t i = 0; i < n; i++) {
    printf(" 0x%02x", ssfe->ssfe_data[i]);
  }
  printf(".\n");
#endif
  if (m_onSendFailed)
  {
    m_onSendFailed(ssfe->ssfe_data, ssfe->ssfe_length);
  }
  return;
}

void peer_connection::handle_notification(struct socket *sock, union sctp_notification *notif, size_t n)
{
  if (notif->sn_header.sn_length != (uint32_t)n) {
    return;
  }
  switch (notif->sn_header.sn_type) {
  case SCTP_ASSOC_CHANGE:
    handle_association_change_event(sock, &(notif->sn_assoc_change));
    break;
  case SCTP_PEER_ADDR_CHANGE:
    handle_peer_address_change_event(&(notif->sn_paddr_change));
    break;
  case SCTP_REMOTE_ERROR:
    handle_remote_error_event(&(notif->sn_remote_error));
    break;
  case SCTP_SHUTDOWN_EVENT:
    handle_shutdown_event(&(notif->sn_shutdown_event));
    break;
  case SCTP_ADAPTATION_INDICATION:
    handle_adaptation_indication(&(notif->sn_adaptation_event));
    break;
  case SCTP_PARTIAL_DELIVERY_EVENT:
    break;
  case SCTP_AUTHENTICATION_EVENT:
    break;
  case SCTP_SENDER_DRY_EVENT:
    break;
  case SCTP_NOTIFICATIONS_STOPPED_EVENT:
    break;
  case SCTP_SEND_FAILED_EVENT:
    handle_send_failed_event(&(notif->sn_send_failed_event));
    break;
  case SCTP_STREAM_RESET_EVENT:
    handle_stream_reset_event(&(notif->sn_strreset_event));
    send_outgoing_stream_reset();
    request_more_o_streams();
    break;
  case SCTP_ASSOC_RESET_EVENT:
    break;
  case SCTP_STREAM_CHANGE_EVENT:
    handle_stream_change_event(&(notif->sn_strchange_event));
    send_outgoing_stream_reset();
    request_more_o_streams();
    break;
  default:
    break;
  }
}

void peer_connection::print_status()
{
  lock_peer_connection();

  struct sctp_status status;
  socklen_t len;
  uint32_t i;
  struct channel *channel;

  len = (socklen_t)sizeof(struct sctp_status);
  if (usrsctp_getsockopt(m_sock, IPPROTO_SCTP, SCTP_STATUS, &status, &len) < 0) {
    perror("getsockopt");
    return;
  }
  std::ostringstream ostr;

  ostr << "Association state: ";
  switch (status.sstat_state) {
  case SCTP_CLOSED:
    ostr << "CLOSED";
    break;
  case SCTP_BOUND:
    ostr << "BOUND";
    break;
  case SCTP_LISTEN:
    ostr << "LISTEN";
    break;
  case SCTP_COOKIE_WAIT:
    ostr << "COOKIE_WAIT";
    break;
  case SCTP_COOKIE_ECHOED:
    ostr << "COOKIE_ECHOED";
    break;
  case SCTP_ESTABLISHED:
    ostr << "ESTABLISHED";
    break;
  case SCTP_SHUTDOWN_PENDING:
    ostr << "SHUTDOWN_PENDING";
    break;
  case SCTP_SHUTDOWN_SENT:
    ostr << "SHUTDOWN_SENT";
    break;
  case SCTP_SHUTDOWN_RECEIVED:
    ostr << "SHUTDOWN_RECEIVED";
    break;
  case SCTP_SHUTDOWN_ACK_SENT:
    ostr << "SHUTDOWN_ACK_SENT";
    break;
  default:
    ostr << "UNKNOWN";
    break;
  }
  VLOG(2) << ostr.str();
  ostr.clear();
  ostr.str("");
  VLOG(2) << "Number of streams (i/o) = (" << status.sstat_instrms << "/" << status.sstat_outstrms;
  for (i = 0; i < NUMBER_OF_CHANNELS; i++) {
    channel = &(m_channels[i]);
    if (channel->state == DATA_CHANNEL_CLOSED) {
      continue;
    }
    ostr << "Channel with id = " << channel->id << ": state ";
    switch (channel->state) {
    case DATA_CHANNEL_CLOSED:
      ostr << "CLOSED";
      break;
    case DATA_CHANNEL_CONNECTING:
      ostr << "CONNECTING";
      break;
    case DATA_CHANNEL_OPEN:
      ostr << "OPEN";
      break;
    case DATA_CHANNEL_CLOSING:
      ostr << "CLOSING";
      break;
    default:
      ostr << "UNKNOWN(" << channel->state << ")";
      break;
    }
    ostr << ", stream id (in/out): (" << channel->i_stream << "/" << channel->o_stream << ") ";

    if (channel->unordered) {
      ostr << "unordered, ";
    } else {
      ostr << "ordered, ";
    }
    switch (channel->pr_policy) {
    case SCTP_PR_SCTP_NONE:
      ostr << "reliable.";
      break;
    case SCTP_PR_SCTP_TTL:
      ostr << "unreliable (timeout " << channel->pr_value << "ms)";
      break;
    case SCTP_PR_SCTP_RTX:
        ostr << "unreliable (max. " << channel->pr_value << " rtx)";
      break;
    default:
      ostr << "unkown policy " << channel->pr_policy;
      break;
    }
    VLOG(2) << ostr.str();
  }

  unlock_peer_connection();
}

int peer_connection::send_open_request_message(struct socket *sock, uint16_t o_stream, bool bUnordered, PrPolicy ePrPolicy, uint32_t pr_value)
{
  /* XXX: This should be encoded in a better way */
  struct rtcweb_datachannel_open_request req;
  struct sctp_sndinfo sndinfo;

  memset(&req, 0, sizeof(struct rtcweb_datachannel_open_request));
  req.msg_type = DATA_CHANNEL_OPEN_REQUEST;
  switch (ePrPolicy) {
  case SCTP_PR_SCTP_NONE:
    /* XXX: What about DATA_CHANNEL_RELIABLE_STREAM */
    req.channel_type = DATA_CHANNEL_RELIABLE;
    break;
  case SCTP_PR_SCTP_TTL:
    /* XXX: What about DATA_CHANNEL_UNRELIABLE */
    req.channel_type = DATA_CHANNEL_PARTIAL_RELIABLE_TIMED;
    break;
  case SCTP_PR_SCTP_RTX:
    req.channel_type = DATA_CHANNEL_PARTIAL_RELIABLE_REXMIT;
    break;
  default:
    return (0);
  }
  req.flags = htons(0);
  if (bUnordered) {
    req.flags |= htons(DATA_CHANNEL_FLAG_OUT_OF_ORDER_ALLOWED);
  }
  req.reliability_params = htons((uint16_t)pr_value); /* XXX Why 16-bit */
  req.priority = htons(0); /* XXX: add support */
  memset(&sndinfo, 0, sizeof(struct sctp_sndinfo));
  sndinfo.snd_sid = o_stream;
  sndinfo.snd_flags = SCTP_EOR;
  sndinfo.snd_ppid = htonl(DATA_CHANNEL_PPID_CONTROL);
  if (usrsctp_sendv(sock,
                    &req, sizeof(struct rtcweb_datachannel_open_request),
                    NULL, 0,
                    &sndinfo, (socklen_t)sizeof(struct sctp_sndinfo),
                    SCTP_SENDV_SNDINFO, 0) < 0) {
    perror("sctp_sendv");
    return (0);
  } else {
    return (1);
  }
}

int peer_connection::send_open_response_message(struct socket *sock, uint16_t o_stream, uint16_t i_stream)
{
  /* XXX: This should be encoded in a better way */
  struct rtcweb_datachannel_open_response rsp;
  struct sctp_sndinfo sndinfo;

  memset(&rsp, 0, sizeof(struct rtcweb_datachannel_open_response));
  rsp.msg_type = DATA_CHANNEL_OPEN_RESPONSE;
  rsp.error = 0;
  rsp.flags = htons(0);
  rsp.reverse_stream = htons(i_stream);
  memset(&sndinfo, 0, sizeof(struct sctp_sndinfo));
  sndinfo.snd_sid = o_stream;
  sndinfo.snd_flags = SCTP_EOR;
  sndinfo.snd_ppid = htonl(DATA_CHANNEL_PPID_CONTROL);
  if (usrsctp_sendv(sock,
                    &rsp, sizeof(struct rtcweb_datachannel_open_response),
                    NULL, 0,
                    &sndinfo, (socklen_t)sizeof(struct sctp_sndinfo),
                    SCTP_SENDV_SNDINFO, 0) < 0) {
    perror("sctp_sendv");
    return (0);
  } else {
    return (1);
  }
}

int peer_connection::send_open_ack_message(struct socket *sock, uint16_t o_stream)
{
  /* XXX: This should be encoded in a better way */
  struct rtcweb_datachannel_ack ack;
  struct sctp_sndinfo sndinfo;

  memset(&ack, 0, sizeof(struct rtcweb_datachannel_ack));
  ack.msg_type = DATA_CHANNEL_ACK;
  memset(&sndinfo, 0, sizeof(struct sctp_sndinfo));
  sndinfo.snd_sid = o_stream;
  sndinfo.snd_flags = SCTP_EOR;
  sndinfo.snd_ppid = htonl(DATA_CHANNEL_PPID_CONTROL);
  if (usrsctp_sendv(sock,
                    &ack, sizeof(struct rtcweb_datachannel_ack),
                    NULL, 0,
                    &sndinfo, (socklen_t)sizeof(struct sctp_sndinfo),
                    SCTP_SENDV_SNDINFO, 0) < 0) {
    perror("sctp_sendv");
    return (0);
  } else {
    return (1);
  }
}


bool peer_connection::connect(EndPoint &endPoint)
{
  struct sockaddr_in addr;
  /* operating as client */
  memset(&addr, 0, sizeof(struct sockaddr_in));
  addr.sin_family = AF_INET;
#ifdef HAVE_SIN_LEN
  addr.sin_len = sizeof(struct sockaddr_in);
#endif
  addr.sin_addr.s_addr = inet_addr(endPoint.getAddress().c_str());
  addr.sin_port = htons(endPoint.getPort());
  if (usrsctp_connect(m_sock, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) < 0) {
    perror("connect");
    LOG(WARNING) << "Failed to connect to " << endPoint;
    return false;
  }
  VLOG(2) << "Connected to " << inet_ntoa(addr.sin_addr) << ":" << ntohs(addr.sin_port);

  m_bIsConnected = true;
  return true;
}

bool peer_connection::bind()
{
  memset(&m_addr, 0, sizeof(struct sockaddr_in));
  m_addr.sin_family = AF_INET;
#ifdef HAVE_SIN_LEN
  m_addr.sin_len = sizeof(struct sockaddr_in);
#endif
  m_addr.sin_addr.s_addr = INADDR_ANY;
  m_addr.sin_port = htons(m_uiSctpPort);
  if (usrsctp_bind(m_sock, (struct sockaddr *)&m_addr, sizeof(struct sockaddr_in)) < 0) {
    perror("bind");
    LOG(WARNING) << "Failed to bind SCTP port " << m_uiSctpPort;
    return false;
  }
  return true;
}

bool peer_connection::listen()
{
  struct socket *conn_sock;
  /* operating as server */
  if (usrsctp_listen(m_sock, 1) < 0) {
    perror("listen");
    LOG(WARNING) << "Failed to listen SCTP port " << m_uiSctpPort;
    return false;
  }

  struct sockaddr_in addr;
  socklen_t addr_len = (socklen_t)sizeof(struct sockaddr_in);
  memset(&addr, 0, sizeof(struct sockaddr_in));
  if ((conn_sock = usrsctp_accept(m_sock, (struct sockaddr *)&addr, &addr_len)) == NULL) {
    perror("accept");
    LOG(WARNING) << "Failed to accept SCTP port " << m_uiSctpPort;
    return false;
  }
  usrsctp_close(m_sock);
  m_sock = conn_sock;
  VLOG(2) << "Connected to " << inet_ntoa(addr.sin_addr) << ":" << ntohs(addr.sin_port);

  return true;
}

bool peer_connection::shutdown()
{
  if (usrsctp_shutdown(m_sock, SHUT_WR) < 0) {
    perror("usrsctp_shutdown");
    LOG(WARNING) << "Failed to shutdown SCTP port " << m_uiSctpPort;
    return false;
  }
  return true;
}

#endif

}
}

