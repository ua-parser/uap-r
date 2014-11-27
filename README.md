uaparser
=========
An implementation of [tobie's ua-parser](http://www.uaparser.org/) in R and C++

__Author:__ Oliver Keyes and Alex Suhan<br/>
__License:__ [MIT](http://opensource.org/licenses/MIT)<br/>

Installation
=====
Install uaparser through the usual <code> install.packages() </code> command, and then place <code>regexes.yaml</code>
from the [uap-core](https://github.com/ua-parser/uap-core) repository in the installation directory. In the event
that you don't complete this step, the uaparser will attempt to download the YAML file into a temporary directory
when called, but this is less efficient and not workable for machines without a direct internet connection.

Dependencies
======
* R;
* [Rcpp](http://cran.rstudio.com/web/packages/Rcpp/);
* C++11;
* [downloader](http://cran.r-project.org/web/packages/downloader/index.html);
* The [boost-regex](http://www.boost.org/doc/libs/1_57_0/libs/regex/doc/html/index.html) C++ library;
* The [boost-system](http://www.boost.org/doc/libs/1_57_0/libs/system/doc/index.html) C++ library;
* The [libyaml-cpp 0.3](https://code.google.com/p/yaml-cpp/) C++ library;
* and the [glog](https://code.google.com/p/google-glog/) C++ library.

Contributing upstream
======
Both the [library](https://github.com/ua-parser/uap-r) and the
[regex definitions](https://github.com/ua-parser/uap-core) welcome patches and reported issues! Just submit through GitHub and we'll do our best to merge or fix them promptly.