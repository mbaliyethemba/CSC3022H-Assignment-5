#ifndef AUDIO_H
#define AUDIO_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdint>
#include <algorithm>

namespace SHNMBA004{
	//Generic parameters
	template <typename BitType>
	class audio{
		private:
			std::vector<BitType> audioData;
			int noOfChannels;
			int sampleRateInHz;
			int noOfSamples;
			int lenOfAudioSeconds;
			
			//loads audio file using binary reading
			void loadAudio(const std::string &filename){
				std::ifstream input(filename,std::ios::binary|std::ios::in);
				if(input.is_open()){
					long fileSize = filesize(filename);
					this->noOfSamples = fileSize/(sizeof(BitType) * noOfChannels);
					this->lenOfAudioSeconds = (int)(noOfSamples / ((float) sampleRateInHz));
					audioData.resize(noOfSamples);
					
					for(int i = o; i < noOfSamples; i++){
						char buffer[sizeof(BitType)];
						BitType sample;
						input.read((char *) buffer, sizeof(BitType)); //reads in the sample
						if(noOfChannels == 2){
							char buffer2[sizeof(BitType)];
							BitType sample2;
							input.read((char *) buffer2, sizeof(BitType)); //reads in sample from right ear
						}
						else{
							audioData[i] = (*(BitType*) buffer);
						}
					}
					input.close();
				}
				else{
					std::cout<<"Cannot open file"<<std::endl;
				}
			}
			
	}
}
