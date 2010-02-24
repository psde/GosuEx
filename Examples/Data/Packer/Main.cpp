#include <Gosu/AutoLink.hpp>
#include <Gosu/IO.hpp>
#include <GosuEx/Data/Archive.hpp>
#include <Gosu/Utility.hpp>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <iostream>

namespace po = boost::program_options;

int main(int argc, char* argv[]) {
		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_EVERY_1024_DF);
		po::options_description desc("Allowed options");
		desc.add_options()
				("help,h", "produce help message")
				("archive,A", po::value<std::string>(), "filename of the archive")
				("filename,F", po::value<std::vector<std::string> >(), "name of a file in the archive or outside of it")
				("add,a", "adds something to the archive")
				("remove,r", "removes something from the archive")
				("save,s", "extracts a file from the archive (all directories must exist)")
				("list,l", "lists the files and the size (in bytes)");
		po::positional_options_description p;
		p.add("archive", 1);
		p.add("filename", -1);
		po::variables_map vm;
		po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
		po::notify(vm);

		if (!vm.count("archive") || (!vm.count("filename") && !vm.count("list")) || vm.count("help")) {
				std::cout << desc << std::endl;
				return 0;
		}

		std::vector<std::string> files;
		if (!vm.count("list")) {
				files = vm["filename"].as<std::vector<std::string> >();
				for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it)
						std::cout << "Filename: " << *it << std::endl;
		}

		std::string archiveName = vm["archive"].as<std::string>();
		std::cout << "Archive: " << archiveName << std::endl;
		GosuEx::Data::Archive archive;

		if (boost::filesystem::exists(archiveName)) {
				std::cout << "Archive exists" << std::endl;
				try {
						archive.load(Gosu::widen(archiveName));
				} catch (std::runtime_error& e) {
						std::cout << e.what() << std::endl;
						return -1;
				} 
		} else
				std::cout << "Archive does not exist" << std::endl;

		if (vm.count("add")) {
				std::cout << "Adding " << files.size() << " files to " << archiveName << std::endl;
				for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it) {
						std::cout << "Processing " << *it << std::endl;
						if (!boost::filesystem::exists(*it)) {
								std::cout << "File does not exist!" << std::endl;
								continue;
						}
						std::cout << "Adding " << *it << " to the archive..." << std::endl;
						archive.addFile(Gosu::widen(*it), Gosu::widen(*it));
				}
				std::cout << "Saving the archive..." << std::endl;
				archive.save(Gosu::widen(archiveName));
				std::cout << "Done!" << std::endl;
		} else if (vm.count("remove")) {
				std::cout << "Removing " << files.size() << " files from " << archiveName << std::endl;
				for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it) {
						std::cout << "Processing " << *it << std::endl;
						archive.remove(Gosu::widen(*it));
				}
				std::cout << "Saving the archive..." << std::endl;
				archive.save(Gosu::widen(archiveName));
				std::cout << "Done!" << std::endl;
		} else if (vm.count("save")) {
				std::cout << "Extracting " << files.size() << " files from " << archiveName << std::endl;
				for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it) {
						std::cout << "Processing " << *it << std::endl;
						try {
								Gosu::saveFile(archive.get(Gosu::widen(*it)), Gosu::widen(*it));
						} catch (std::runtime_error& e) {
								std::cout << "ERROR! Could not save " << *it << ": " << e.what() << std::endl;
								continue;
						}
				} 
				std::cout << "Saving the archive..." << std::endl;
				archive.save(Gosu::widen(archiveName));
				std::cout << "Done!" << std::endl;
		} else if (vm.count("list")) {
				std::cout << "Listing " << archive.size() << " files from " << archiveName << std::endl;
				std::cout << "File/Buffer     | Size (Bytes)" << std::endl;
				for (GosuEx::Data::Archive::iterator it = archive.begin(); it != archive.end(); ++it) {
						std::cout << (boost::format("%15s | %-8d") % Gosu::narrow(*it) % archive.get(*it).size()).str() << std::endl;
				}
				std::cout << "Done!" << std::endl;
		}
}