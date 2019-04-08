#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "GPSWeekSecond.hpp"
#include "BasicFramework.hpp"
#include "PRSolution.hpp"

#include "EGNOS_EMS_Data.hpp"
#include "EGNOS_EMS_Stream.hpp"

#include "IGPMap.hpp"
#include "IonosphericMaskBand.hpp"
#include "IonosphericGridPoint.hpp"

#include "GINAConfig.h"

#include "IGPMap.hpp"
#include "IonoCorrection.hpp"

using namespace std;

template<std::size_t R, std::size_t L, std::size_t N>
std::bitset<N> project_range(std::bitset<N> b)
{
	static_assert(R <= L && L <= N, "invalid bitrange");
	b >>= R;            // drop R rightmost bits
	b <<= (N - L + R);  // drop L-1 leftmost bits
	b >>= (N - L);      // shift back into place
	return b;
}

int main(int argc, char **argv) {


	std::string EDAS_FileNamewPath = ROOT "\\70_EGNOS_Project\\files\\h17.ems";
	
	EGNOS_EMS_Parser::EGNOS_EMS_Stream exampleStreamIn(EDAS_FileNamewPath.c_str());

	EGNOS_EMS_Parser::EGNOS_EMS_Data EData;

	EGNOS::IonosphericDelayCorrectionsMessageParser IonoGridPointParser;
	EGNOS::IonosphericGridPointMasksMessageParser IonoMaskParser;

	EGNOS::IGPMap IonoMap;
	EGNOS::IonexCreator ionexWriter;
	EGNOS::IGPMediator IgpMediator;

	

	bool weHad18 = false;
	bool weHad26 = false;

	gpstk::CommonTime CurrentDataTime;

	int updateIndex = 0;
	while (exampleStreamIn >> EData) {

		CurrentDataTime = EData.messageTime;

		if (EData.messageId == 18) {

			IonoMaskParser += EData.message;
			//cout << IonoMaskParser << endl;
			weHad18 = true;
		}

		if (EData.messageId == 26) {

			IonoGridPointParser += EData.message;
			//cout << IonoGridPointParser << endl;
			weHad26 = true;
		}

		if (weHad18 || weHad26) {

			bool newData = false;

			IgpMediator.updateTime(CurrentDataTime);
			IgpMediator.setIGPCandidates(IonoGridPointParser.getIonosphericGridPoint());
			IgpMediator.updateIGPCandidate(IonoMaskParser);
			
			newData = IonoMap.updateMap(IgpMediator);

			if (newData == true) {
				updateIndex++;
				string ems_out_file_index = std::to_string(updateIndex);
				std::string ionexFile_Out = ROOT "\\70_EGNOS_Project\\files\\Ionex_from_h17\\h17ems_ionex_out_" + ems_out_file_index + ".18i";
				
				ionexWriter.setIonexData(IonoMap);
				ionexWriter.write2file(ionexFile_Out);
				cout << "IGP Map updated and file created" << endl;
			}
			//cout << IonoMap;

			weHad26 = false;
			weHad18 = false;
		}
	}

	//cout << IonoMap;

	exampleStreamIn.close();

	return 0;
}