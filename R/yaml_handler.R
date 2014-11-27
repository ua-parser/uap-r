#'@importFrom downloader download
yaml_handler <- function(){
  
  #Grab the (likely) path to the YAML file, and if it exists there, return that
  lib_loc <- file.path(find.package("uaparser"),"regexes.yaml")
  if(file.exists(lib_loc)){
    return(lib_loc)
  }
  
  #If it doesn't, warn and attempt to download temporarily
  warning("You do not have the YAML file stored locally - see ?uaparser.\n Attempting to temporarily download the file.\n")
  
  destfile <- tempfile(pattern = "fake_yaml", tmpdir = tempdir(), fileext = "")
  con <- download(url = "https://raw.githubusercontent.com/ua-parser/uap-core/master/regexes.yaml",
                  destfile = destfile, quiet = TRUE)
  if(con != 0){
    stop("The file could not be downloaded. Please download it and store it locally - see ?uaparser")
  }
  
  #Assuming nothing went wrong with the download, return the filename
  return(destfile)                     
  
}