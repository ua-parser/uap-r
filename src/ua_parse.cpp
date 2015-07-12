#include <Rcpp.h>
#include "internal_ua_parser.h"
using namespace Rcpp;

//Return checker - checks that an empty string wasn't returned. Easy enough.
std::string return_checker(std::string returned_string){
  std::string to_output = "Other";
  if(returned_string == ""){
    return to_output;
  }
  return returned_string;
}

// Internal C++ function for parsing user agents.
// [[Rcpp::export]]
DataFrame ua_parse_(std::vector < std::string > agents, std::string yaml_file_loc) {
  
  //Load regex file
  ua_parser::Parser uap(yaml_file_loc);
  
  //Measure input size, create output and holding objects
  int agents_size = agents.size();
  ua_parser::UserAgent holding;
  std::vector < std::string > device_family(agents_size);
  std::vector < std::string > os(agents_size);
  std::vector < std::string > os_major(agents_size);
  std::vector < std::string > os_minor(agents_size);
  std::vector < std::string > os_patch(agents_size);
  std::vector < std::string > os_patch_minor(agents_size);
  std::vector < std::string > browser_family(agents_size);
  std::vector < std::string > browser_major(agents_size);
  std::vector < std::string > browser_minor(agents_size);
  std::vector < std::string > browser_patch(agents_size);
  std::vector < std::string > browser_patch_minor(agents_size);

  //For each agent...
  for(int i = 0; i < agents_size; i++) {
    
    //Identify it
    holding = uap.Parse(agents[i]);
    
    //Throw the results into the relevant vectors
    device_family[i] = holding.device;
    os[i] = holding.os.os;
    os_major[i] = return_checker(holding.os.major);
    os_minor[i] = return_checker(holding.os.minor);
    os_patch[i] = return_checker(holding.os.patch);
    os_patch_minor[i] = return_checker(holding.os.patch_minor);
    browser_family[i] = holding.browser.family;
    browser_major[i] = holding.browser.major;
    browser_minor[i] = holding.browser.minor;
    browser_patch[i] = return_checker(holding.browser.patch);
    browser_patch_minor[i] = return_checker(holding.browser.patch_minor);
  }
  
  //Create DF out of the resulting objects and return it
  return DataFrame::create(_["device"] = device_family, _["os"] = os,
                           _["os_major"] = os_major, _["os_minor"] = os_minor,
                           _["os_patch"] = os_patch, _["os_patch_minor"] = os_patch_minor,
                           _["browser"] = browser_family, _["browser_major"] = browser_major,
                           _["browser_minor"] = browser_minor, _["browser_patch"] = browser_patch,
                           _["browser_patch_minor"] = browser_patch_minor, _["stringsAsFactors"] = false);
  
}

std::string extract_first_match(std::string agent, boost::regex regex_to_match){
  
  boost::smatch result;
  std::string output = "Other";
  if(boost::regex_search(agent, result, regex_to_match)){
    output = result[0];
  }
  return output;
}

//[[Rcpp::export]]
DataFrame parse_r_agents_(std::vector < std::string > user_agents){
  
  boost::regex r_version("(?<=^R \\()\\d\\.\\d{1,2}\\.\\d");
  boost::regex architecture("(i386|x86_64|i686|i486)");
  boost::regex platform("(mingw32|linux|apple)");
  
  int input_size = user_agents.size();
  std::vector < std::string > output_versions(input_size);
  std::vector < std::string > output_archs(input_size);
  std::vector < std::string > output_platforms(input_size);
  std::string holding;
  
  for(int i = 0; i < input_size; i++){
    output_versions[i] = extract_first_match(user_agents[i], r_version);
    output_archs[i] = extract_first_match(user_agents[i], architecture);
    holding = extract_first_match(user_agents[i], platform);
    if(holding == "mingw32"){
      holding = "windows";
    }
    output_platforms[i] = holding;
  }
  
  return DataFrame::create(_["r_version"] = output_versions, _["architecture"] = output_archs,
                           _["platform"] = output_platforms, _["stringsAsFactors"] = false);
}