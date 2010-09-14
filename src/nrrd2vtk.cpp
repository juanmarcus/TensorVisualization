#include "vtkPNrrdReader.h"
#include "vtkImageData.h"
#include "vtkXMLImageDataWriter.h"
#include "boost/program_options.hpp"

namespace po = boost::program_options;

int main(int argc, char **argv)
{
	//  Command line options
	std::string infilename;
	std::string outfilename;
	po::options_description desc("Options");
	desc.add_options()("input,i", po::value<std::string>(&infilename),
			"Input file to convert")("output,o", po::value<std::string>(
			&outfilename), "Output file");

	//	Parse command line
	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);
	if (vm.count("input") == 0)
	{
		std::cout << desc << std::endl;
		return 0;
	}
	if (vm.count("output") == 0)
	{
		std::cout << desc << std::endl;
		return 0;
	}

	//	Input data
	vtkPNrrdReader* nrrdReader = vtkPNrrdReader::New();
	nrrdReader->SetFileName(infilename.c_str());
	nrrdReader->Update();

	nrrdReader->Update();
	nrrdReader->GetOutput()->Print(std::cout);

	// Output data
	vtkXMLImageDataWriter* writer = vtkXMLImageDataWriter::New();
	writer->SetFileName(outfilename.c_str());
	writer->SetInputConnection(nrrdReader->GetOutputPort());
	writer->Write();

	return 0;
}

