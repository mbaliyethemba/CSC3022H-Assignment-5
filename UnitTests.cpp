#define CATCH_CONFIGUR_MAIN
#include "catch.hpp"
#include "audio.h"

using namespace std;
using namespace SHNMBA004;

TEST_CASE("Test : Special Members"){
	SECTION("Constructor"){
		string filename = "sample_input/beez18sec_44100_signed_8bit_mono.raw";
		int channel = 1;
		int srate = 44100;
		audio<int8_t> audLoad = audio<int8_t>(filename,channel,srate);
		REQUIRE((audLoad.getNoOfChannels() == channel));
		REQUIRE((audLoad.getLenInSec() == 18));
		REQUIRE((audLoad.getNoOfSamples() == 793800));
		REQUIRE((audLoad.getSampleRate() == srate));
	}
	
	SECTION("Destructor"){
		string filename = "sample_input/countdown40sec_44100_signed_8bit_mono.raw";
		int channel = 1;
		int srate = 44100;
		audio<int8_t> *audLoad = new audio<int8_t>(filename,channel,srate);
		delete audLoad;
		 //Check if data cleared
		REQUIRE(audLoad->checkIfAudioEmpty());
	}
	
	SECTION("Copy/Move Constructor"){
		string filename = "sample_input/frogs18sec_44100_signed_8bit_mono.raw";
		int channel = 1;
		int srate = 44100;
		audio<int8_t> audLoad = audio<int8_t>(filename,channel,srate);
		audio<int8_t> copyAud(audLoad);
		REQUIRE((audLoad == copyAud));
		audio<int8_t> movedAud(move(audLoad));
		REQUIRE((audLoad.getNoOfChannels() == 0));
		REQUIRE((audLoad.getLenInSec() == 0));
		REQUIRE((audLoad.getNoOfSamples() == 0));
		REQUIRE((audLoad.getSampleRate() == 0));
		REQUIRE(audLoad.checkIfAudioEmpty());
	}
	
	SECTION("Copy/Move Assignment Operators"){
		string filename = "sample_input/siren40sec_44100_signed_8bit_mono.raw";
		int channel = 1;
		int srate = 44100;
		audio<int8_t> audLoad = audio<int8_t>(filename,channel,srate);
		audio<int8_t> copyAud = audLoad;
		REQUIRE((audLoad == copyAud));
		audio<int8_t> movedAud = move(audLoad);
		REQUIRE((audLoad == movedAud));
		REQUIRE((audLoad.getNoOfChannels() == 0));
		REQUIRE((audLoad.getLenInSec() == 0));
		REQUIRE((audLoad.getNoOfSamples() == 0));
		REQUIRE((audLoad.getSampleRate() == 0));
		REQUIRE(audLoad.checkIfAudioEmpty());
	}
}

TEST_CASE("Saving and Loading Audio"){
	SECTION("Mono Test"){
		string filename = "sample_input/beez18sec_44100_signed_8bit_mono.raw";
		int channel = 1;
		int srate = 44100;
		audio<int8_t> audLoad = audio<int8_t>(filename,channel,srate);
		audLoad.saveAudio("beezM");
		string filesaved = "beezM_44100_8_mono.raw";
		audio<int8_t> audSaved = audio<int8_t>(filesaved,channel,srate);
		REQUIRE(audLoad == audSaved);
	}
	
	SECTION("Stereo Test"){
		string filename = "sample_input/beez18sec_44100_signed_8bit_stereo.raw";
		int channel = 2;
		int srate = 44100;
		audio<pair<int8_t,int8_t>> audLoad = audio<pair<int8_t,int8_t>>(filename,channel,srate);
		audLoad.saveAudio("beezS");
		string filesaved = "beezS_44100_8_stereo.raw";
		audio<pair<int8_t,int8_t>> audSaved = audio<pair<int8_t,int8_t>>(filesaved,channel,srate);
		REQUIRE(audLoad == audSaved);
	}
}
