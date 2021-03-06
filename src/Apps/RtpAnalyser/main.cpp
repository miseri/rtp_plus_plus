#include "RtpAnalyserPch.h"
#include <fstream>
#include <sstream>
#include <string>
#include <boost/exception/all.hpp>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <cpputil/ConsoleApplicationUtil.h>
#include <cpputil/GenericParameters.h>
#include <rtp++/media/SimpleMultimediaService.h>
#include <rtp++/mediasession/MediaSessionDescription.h>
#include <rtp++/mediasession/SimpleMediaSession.h>
#include <rtp++/mediasession/RtpAnalysisMediaSessionFactory.h>
#include <rtp++/application/Application.h>
#include <rtp++/rfc3550/Rfc3550.h>

using namespace std;
using namespace rtp_plus_plus;
using namespace rtp_plus_plus::app;

namespace po = boost::program_options;

int main(int argc, char** argv)
{
  if (!ApplicationContext::initialiseApplication(argv[0]))
  {
    LOG(WARNING) << "Failed to initialise application";
    return 0;
  }

  // read local SDP
  try
  {
    ApplicationConfiguration ac;
    ac.addOptions(ApplicationConfiguration::GENERIC);
    ac.addOptions(ApplicationConfiguration::SESSION);
    ac.addOptions(ApplicationConfiguration::RECEIVER);

    po::options_description cmdline_options = ac.generateOptions();

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).
              options(cmdline_options).run(), vm);
    po::notify(vm);

    if (vm.count("help"))
    {
      ostringstream ostr;
      ostr << cmdline_options;
      LOG(ERROR) << ostr.str();
      return 1;
    }

    if (ac.Application.logfile != "")
    {
      // update the log file: we want to be able to parse this file
      google::SetLogDestination(google::GLOG_INFO, (ac.Application.logfile + ".INFO").c_str());
      google::SetLogDestination(google::GLOG_WARNING, (ac.Application.logfile + ".WARNING").c_str());
      google::SetLogDestination(google::GLOG_ERROR, (ac.Application.logfile + ".ERROR").c_str());
    }

    ApplicationUtil::printApplicationInfo(argc, argv);

    // generic parameters
    GenericParameters applicationParameters = ac.getGenericParameters();

    boost::optional<InterfaceDescriptions_t> localConf = ApplicationUtil::readNetworkConfigurationFile(ac.Session.NetworkConf.at(0), false);
    if (!localConf) { return -1; }

    boost::optional<rfc4566::SessionDescription> sdp = ApplicationUtil::readSdp(ac.Session.sessionDescriptions.at(0));
    if (!sdp) { return -1; }

    // perform SDP to RtpSessionParameter translation
    MediaSessionDescription mediaSessionDescription = MediaSessionDescription(rfc3550::getLocalSdesInformation(), *sdp, *localConf, false);
    RtpAnalysisMediaSessionFactory factory;
    boost::shared_ptr<SimpleMediaSession> pMediaSession = factory.create(mediaSessionDescription, applicationParameters);

    // the user can now specify the handlers for incoming samples.
    startEventLoop(boost::bind(&SimpleMediaSession::start, pMediaSession), boost::bind(&SimpleMediaSession::stop, pMediaSession));

    if (!ApplicationContext::cleanupApplication())
    {
      LOG(WARNING) << "Failed to cleanup application cleanly";
    }

    return 0;
  }
  catch (boost::exception& e)
  {
    LOG(ERROR) << "Exception: %1%" << boost::diagnostic_information(e);
  }
  catch (std::exception& e)
  {
    LOG(ERROR) << "Exception: %1%" << e.what();
  }
  catch (...)
  {
    LOG(ERROR) << "Unknown exception!!!";
  }

  if (!ApplicationContext::cleanupApplication())
  {
    LOG(WARNING) << "Failed to cleanup application cleanly";
  }
  return 0;
}
