#'@title Update Regular Expressions
#'@description \code{uaparser} includes a set of regular expressions used to parse user agents.
#'These will be updated on every new version of the package, but can still get out of date
#'in the meantime. \code{update_regexes} replaces the version of the regex file with
#'the latest version from the central UA-Parser repository.
#'
#'@seealso \code{\link{parse_agents}}, which relies on the underlying regex file.
#'
#'@import httr
#'@export
update_regexes <- function(){
  file_location <- system.file("extdata/regexes.yaml", package = "uaparser")
  result <- httr::GET("https://raw.githubusercontent.com/ua-parser/uap-core/master/regexes.yaml")
  httr::stop_for_status(result)
  result <- httr::content(result)
  cat(result, file = file_location)
  return(invisible())
}

#'@title Parse User Agents
#'@description Take a vector of user agents and parse
#'them into their component parts.
#'
#'
#'@param user_agents a vector of user agents
#'
#'@param fields a vector of the fields you want to return - options are "device", "os", "os_major", "os_minor",
#'"os_patch","os_patch_minor", "browser", "browser_major", "browser_minor", "browser_patch" and "browser_patch_minor",
#'where minor/major refer to version numbers. By default, all fields are returned.
#'
#'@return a data.frame consisting of those fields you wanted to return.
#'
#'@seealso \code{\link{update_regexes}}, for updating the regular expressions file.
#'
#'@export
parse_agents <- function(user_agents, fields = NULL){
  
  #Parse
  parse_results <- ua_parse_(agents = user_agents, yaml_file_loc = system.file("extdata/regexes.yaml", package = "uaparser"))
  
  #If the fields requested are NULL, just return the whole thing
  if(is.null(fields)){
    return(parse_results)
  }
  
  #Otherwise, limit and return
  return(parse_results[,fields])
}