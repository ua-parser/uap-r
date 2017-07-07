/*
# Copyright 2013 Andrew Punch
#
# Licensed under the Apache License, Version 2.0 (the 'License')
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#         http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an 'AS IS' BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
*/

#include <fstream>
#include <boost/algorithm/string/replace.hpp>
#include <yaml-cpp/yaml.h>
#include <Rcpp.h>
#include "internal_ua_parser.h"

namespace ua_parser {
BrowserParser::BrowserParser(const YAML::Node &yaml_attributes) {
  pattern = yaml_attributes["regex"].as<std::string>();
  regex = boost::regex(pattern);

  try { // This try-catch block can be removed when the const bug in yaml-cpp is fixed.
    if (yaml_attributes["family_replacement"]) {
      overrides.family = yaml_attributes["family_replacement"].as<std::string>();
    }
  } catch (const YAML::InvalidNode& e) { /* No replacement. */ }
  try { // This try-catch block can be removed when the const bug in yaml-cpp is fixed.
    if (yaml_attributes["v1_replacement"]) {
      overrides.major = yaml_attributes["v1_replacement"].as<std::string>();
    }
  } catch (const YAML::InvalidNode& e) { /* No replacement. */ }
  try { // This try-catch block can be removed when the const bug in yaml-cpp is fixed.
    if (yaml_attributes["v2_replacement"]) {
      overrides.minor = yaml_attributes["v2_replacement"].as<std::string>();
    }
  } catch (const YAML::InvalidNode& e) { /* No replacement. */ }
}



Browser BrowserParser::Parse(const std::string &user_agent_string) {
  Browser browser;
  using namespace std;

  boost::smatch result;
  if (boost::regex_search(user_agent_string.begin(), user_agent_string.end(), result, regex)) {
    if (overrides.family.empty() && result.size()>1)
      browser.family = result[1].str();
    else {
      browser.family = overrides.family;
      boost::replace_all(browser.family, "$1", result[1].str());
    }

    // If the group is not found, use the override value even if it is blank

    if (overrides.major.empty()&&result.size()>2)
      browser.major = result[2].str();
    else
      browser.major = overrides.major;

    if (overrides.minor.empty()&&result.size()>3)
      browser.minor = result[3].str();
    else
      browser.minor = overrides.minor;

    if (overrides.patch.empty()&&result.size()>4)
      browser.patch = result[4].str();
    else
      browser.patch = overrides.patch;

    return browser;
  }

  return browser;

}

OperatingSystemParser::OperatingSystemParser(const YAML::Node &yaml_attributes) {
  pattern = yaml_attributes["regex"].as<std::string>();
  regex = boost::regex(pattern);

  try { // This try-catch block can be removed when the const bug in yaml-cpp is fixed.
    if (yaml_attributes["os_replacement"]) {
      overrides.os = yaml_attributes["os_replacement"].as<std::string>();
    }
  } catch (const YAML::InvalidNode& e) { /* No replacement. */ }
  try { // This try-catch block can be removed when the const bug in yaml-cpp is fixed.
    if (yaml_attributes["os_v1_replacement"]) {
      overrides.major = yaml_attributes["os_v1_replacement"].as<std::string>();
    }
  } catch (const YAML::InvalidNode& e) { /* No replacement. */ }
  try { // This try-catch block can be removed when the const bug in yaml-cpp is fixed.
    if (yaml_attributes["os_v2_replacement"]) {
      overrides.minor = yaml_attributes["os_v2_replacement"].as<std::string>();
    }
  } catch (const YAML::InvalidNode& e) { /* No replacement. */ }
}

OperatingSystem OperatingSystemParser::Parse(const std::string &user_agent_string) {
  OperatingSystem os;
  using namespace std;

  boost::smatch result;
  if (boost::regex_search(user_agent_string.begin(), user_agent_string.end(), result, regex)) {
    if (overrides.os.empty() && result.size()>1)
      os.os = result[1].str();
    else {
      os.os = overrides.os;
      boost::replace_all(os.os, "$1", result[1].str());
    }

    // If the group is not found, use the override value even if it is blank
    if (overrides.major.empty()&&result.size()>2)
      os.major = result[2].str();
    else
      os.major = overrides.major;

    if (overrides.minor.empty()&&result.size()>3)
      os.minor = result[3].str();
    else
      os.minor = overrides.minor;

    if (overrides.patch.empty()&&result.size()>4)
      os.patch = result[4].str();
    else
      os.patch = overrides.patch;

    if (overrides.patch_minor.empty()&&result.size()>5)
      os.patch_minor = result[5].str();
    else
      os.patch_minor = overrides.patch_minor;

    return os;
  }
  return os;
}

DeviceParser::DeviceParser(const YAML::Node &yaml_attributes) {
  pattern = yaml_attributes["regex"].as<std::string>();
  regex = boost::regex(pattern);

  try { // This try-catch block can be removed when the const bug in yaml-cpp is fixed.
    if (yaml_attributes["device_replacement"]) {
      overrides = yaml_attributes["device_replacement"].as<std::string>();
    }
  } catch (const YAML::InvalidNode& e) { /* No replacement. */ }
}

Device DeviceParser::Parse(const std::string &user_agent_string) {
  Device device;
  using namespace std;

  boost::smatch result;
  if (boost::regex_search(user_agent_string.begin(), user_agent_string.end(), result, regex)) {
    if (overrides.empty() && result.size()>1)
      device = result[1].str();
    else {
      device = overrides;
      boost::replace_all(device, "$1", result[1].str());
    }

    return device;
  }

  return device;
}



template<class ParserType>
static std::vector<ParserType> ParseYaml(const YAML::Node &yaml_regexes) {
  std::vector<ParserType> parsers;

  for (auto i=yaml_regexes.begin(); i!=yaml_regexes.end(); i++) {
    ParserType parser(*i);

    parsers.push_back(parser);
  }

  return parsers;
}


Parser::Parser(const std::string &yaml_file) {
  YAML::Node doc = YAML::LoadFile(yaml_file);

  if (doc.IsNull()) {
    Rcpp::stop("Could not open user agent YAML file");
  }


  _browser_parsers = ParseYaml<BrowserParser>(doc["user_agent_parsers"]);
  _os_parsers = ParseYaml<OperatingSystemParser>(doc["os_parsers"]);
  _device_parsers = ParseYaml<DeviceParser>(doc["device_parsers"]);

}

UserAgent Parser::Parse(const std::string &user_agent_string) {
  UserAgent user_agent;

  user_agent.browser.family = "Other";
  user_agent.os.os = "Other";
  user_agent.device = "Other";
  user_agent.browser.major = "Other";
  user_agent.browser.minor = "Other";

  for (std::vector<BrowserParser>::iterator i=_browser_parsers.begin(); i!=_browser_parsers.end(); i++) {
    Browser browser = i->Parse(user_agent_string);
    if (!browser.family.empty()) {
      user_agent.browser = browser;
      break;
    }

  }

  for (std::vector<OperatingSystemParser>::iterator i=_os_parsers.begin(); i!=_os_parsers.end(); i++) {
    OperatingSystem os = i->Parse(user_agent_string);
    if (!os.os.empty()) {
      user_agent.os = os;
      break;
    }

  }

  for (std::vector<DeviceParser>::iterator i=_device_parsers.begin(); i!=_device_parsers.end(); i++) {
    Device device = i->Parse(user_agent_string);
    if (!device.empty()) {
      user_agent.device = device;
      break;
    }

  }



  return user_agent;
}

}

