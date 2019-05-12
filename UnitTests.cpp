#define CATCH_CONFIGUR_MAIN
#include "catch.hpp"
#include "audio.h"

using namespace std;
using namespace SHNMBA004;

TEST_CASE("Test"){
	SECTION("Constructor"){
		string filename = "sample_input/beez18sec_44100_signed_8bit_mono.raw";
		int channel = 1;
		int srate = 44100;
		audio<int8_t> audLoad = audio<int8_t>(filename,channel,rate);
		REQUIRE((audLoad.getNoOfChannels() == channel));
		REQUIRE((audLoad.getLenInSec() == 18));
		REQUIRE((audLoad.getNoOfSamples() == 793800));
		REQUIRE((audLoad.getSampleRate() == srate));
	}
	
}
