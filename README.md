EXTENSIONS
==========
* **Data**:
  Data is an easy interface to store digital data (i.e. savegames or other things). It provides functionality to compress the serialized data using zlib. The stored data can then either be sent with a Gosu::Socket or be stored in a file. For  file storages there are predefined functions available. This is a library based extension.
	
  
INSTALLATION (WINDOWS)
======================
You probably have to add this path here to your include path so that it is possible to include files such as <GosuEx/Data/Data.hpp>. If you want to compile libraries you have to set their path (i.e. GosuEx/Data/) to your include path since most of them will just do "Data.hpp" to include their files (note: this should not be necessary if the given vcproj files are used). If you are using MSVC you will also have to get a workcopy of the Gosu SVN/Gosu git and set its path into your environment variable GOSU_DIR (this is only required to compile the Data package). Of course, you can simply change the project files to avoid that. But it is most likely that you have to get a workout and set it into your default path (i.e. <Gosu/Gosu.hpp> should work). There are no precompiled files available. Visual Studio 2008 and Visual Studio 2010 projects and solutions are included to build the project easily on Windows.	
  
Short:
- Checkout Gosu SVN/clone Gosu git
- Set the workcopy/clone path (i.e. D:\Visual Studio 2008\libs\Gosu\) to the environment variable GOSU_DIR
	To check if it's working, run cmd and try 'cd %GOSU_DIR%'.
- Compile the projects you want to.
  
Short (any other system):
There are currently no makefiles, automagically installers or something like that available (yet).