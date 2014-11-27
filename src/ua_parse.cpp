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

// Internal C function for parsing user agents.
// [[Rcpp::export]]
DataFrame c_ua_parse(std::vector < std::string > agents, std::string yaml_file_loc) {
  
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
    
    //Throw the results into the relevant lists
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
