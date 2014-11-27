#'@title uaparser
#'@description User agent parser
#'
#'@details uaparser (both the function and the package) are the R port of
#'\href{https://github.com/ua-parser/uap-core}{tobie's ua-parser}. Rather than implementing
#'the parser entirely in R (which would be slow for the kind of operations that it performed),
#'this is largely a wrapper around the C++ implementation.
#'
#'@param user_agents a vector of user agents
#'
#'@param fields a vector of the fields you want to return - options are "device", "os", "os_major", "os_minor",
#'"os_patch","os_patch_minor", "browser", "browser_major", "browser_minor", "browser_patch" and "browser_patch_minor",
#'where minor/major refer to version numbers. 
#'
#'@return a data.frame consisting of those fields you wanted to return. In the event that no "fields"
#'parameter was set by the user, all fields will be returned.
#'
#'@export
uaparser <- function(user_agents, fields = NULL){
  
  #Grab YAML location
  yaml_file <- yaml_handler()
  
  #Parse
  parse_results <- c_ua_parse(agents = user_agents, yaml_file_loc = yaml_file)
  
  #If the file looks temporary, eliminate it
  if(grepl(x = yaml_file, pattern = "fake_yaml", fixed = TRUE)){
    file.remove(yaml_file)
  }
  
  #If the fields requested are NULL, just return the whole thing
  if(is.null(fields)){
    return(parse_results)
  }
  
  #Otherwise, limit and return
  return(parse_results[,fields])
}