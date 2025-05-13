#include "outReligion.h"
#include <fstream>

EU4::outReligion::outReligion(const Configuration& theConfiguration,
	 std::vector<mappers::ReformedReligionMapping> unreligionReforms,
	 std::vector<mappers::ReformedReligionMapping> religionReforms)
{
	std::ofstream unReformedReligions(std::filesystem::path("configurables/reformation/dynamicPagans/03_converter_unreformed_religions.txt"));
	if (!unReformedReligions.is_open())
		throw std::runtime_error("Could not create custom unreformed religions file!");

	unReformedReligions << "pagan = {\n\t";
	for (auto unreligion: unreligionReforms)
	{
		unReformedReligions << unreligion.getName() << " = {\n"
								  << "\t\ticon = " << unreligion.getIconNumber() << "\n"
								  << "\t\tcolor " << *unreligion.getColor() << "\n"
								  << "\t\tcountry = {\n\t\t\t" << unreligion.getCountryModifiers() << "\n\t\t}\n"
								  << "\t\tprovince = {\n\t\t\t" << unreligion.getProvinceModifiers() << "\n\t\t}\n\t\t"
								  << "\t\tcountry_as_secondary = {\n\t\t\t" << unreligion.getSecondary() << "\n\t\t}\n\t\t" << unreligion.getUniqueMechanics()
								  << "\n\t\t" << unreligion.getNonUniqueMechanics() << "\n"
								  << "\t\theretic = { " << unreligion.getHereticStrings() << " }\n\t}\n\t";
	}
	unReformedReligions << "\n}";
	unReformedReligions.close();

	std::ofstream reformedReligions(std::filesystem::path("configurables/reformation/dynamicPagans/04_custom_reformed_religions.txt"));
	if (!reformedReligions.is_open())
		throw std::runtime_error("Could not create custom reformed religions file!");

	reformedReligions << "pagan = {\n\t";
	for (auto religion: religionReforms)
	{
		reformedReligions << religion.getName() << " = {\n"
								<< "\t\ticon = " << religion.getIconNumber() << "\n"
								<< "\t\tcolor " << *religion.getColor() << "\n"
								<< "\t\tcountry = {\n\t\t\t" << religion.getCountryModifiers() << "\n\t\t}\n"
								<< "\t\tprovince = {\n\t\t\t" << religion.getProvinceModifiers() << "\n\t\t}\n\t\t"
								<< "\t\tcountry_as_secondary = {\n\t\t\t" << religion.getSecondary() << "\n\t\t}\n\t\t" << religion.getUniqueMechanics() << "\n\t\t"
								<< religion.getNonUniqueMechanics() << "\n"
								<< "\t\theretic = { " << religion.getHereticStrings() << " }\n\t}\n\t";
	}
	reformedReligions << "\n}";
	reformedReligions.close();

	auto files = commonItems::GetAllFilesInFolder(std::filesystem::path("configurables/reformation/dynamicPagans/"));
	for (const auto& file: files)
		std::filesystem::copy_file("configurables/reformation/dynamicPagans" / file.filename(),
			 "output" / theConfiguration.getOutputName() / "common/religions" / file.filename());
}