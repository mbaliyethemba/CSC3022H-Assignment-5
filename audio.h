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
		public:
			//constructor
			audio(const std::string &filename, int &channel, int &rate) : noOfChannels(channel), sampleRateInHz(rate){
				loadAudio(filename);
			}
			
			audio() = default;
			
			//Destructor
			~audio(){
				noOfChannels = 0;
				sampleRateInHz = 0;
				noOfSamples = 0;
				lenOfAudioSeconds = 0;
				audioData.clear();
				std::vector<BitType>().swap(audioData)
			}
			
			//constructor
			audio(int samples, int length, std::vector<BitType> v, int &channel, int &rate) : noOfChannels(channel), sampleRateInHz(rate), noOfSamples(samples), lenOfAudioSeconds(length), audioData(v) {
			}
			
			//Copy constructor
			audio(const audio &rhs) : noOfChannels(rhs.noOfChannels), sampleRateInHz(rhs.sampleRateInHz), noOfSamples(rhs.noOfSamples), lenOfAudioSeconds(rhs.lenOfAudioSeconds){
			}
			
			//Move constructor
			audio(audio &&rhs) : noOfChannels(rhs.noOfChannels), sampleRateInHz(rhs.sampleRateInHz), noOfSamples(rhs.noOfSamples), lenOfAudioSeconds(rhs.lenOfAudioSeconds){
				rhs.noOfChannels = 0;
				rhs.sampleRateInHz = 0;
				rhs.noOfSamples = 0;
				rhs.lenOfAudioSeconds = 0;
				rhs.audioData.clear();
			}
			
			//copy assignment operator
			audio &operator=(const audio & rhs){
				noOfChannels = rhs.noOfChannels;
				sampleRateInHz = rhs.sampleRateInHz;
				noOfSamples = rhs.noOfSamples;
				lenOfAudioSeconds = rhs.lenOfAudioSeconds;
				audioData = rhs.audioData;
			}
			
			//move assignment operator
			audio &operator=(audio && rhs){
				noOfChannels = rhs.noOfChannels;
				sampleRateInHz = rhs.sampleRateInHz;
				noOfSamples = rhs.noOfSamples;
				lenOfAudioSeconds = rhs.lenOfAudioSeconds;
				audioData = rhs.audioData;
				
				rhs.noOfChannels = 0;
				rhs.sampleRateInHz = 0;
				rhs.noOfSamples = 0;
				rhs.lenOfAudioSeconds = 0;
				rhs.audioData.clear();
			}
		//for unit tests
		int getNoOfChannels(){
			return noOfChannels;
		};
		
		int getSampleRate(){
			return sampleRateInHz;
		};
		
		int getNoOfSamples(){
			return noOfSamples
		};
		
		int getLenInSec(){
			return lenOfAudioSeconds;
		};
		
		bool checkIfAudioEmpty(){
			return audioData.empty();
		};
		
		//saves audio data into file
		void saveAudio(const std::string & outfile){
			std::string full = outfile + "_" + std::to_string(sampleRateInHz) + "_" + std::to_string(sizeof(BitType)*8) + "_mono.raw";
			std::ofstream output(full, std::ios::binary | std::ios::out);
			if(output.is_open()){
				for(int i = 0; i < audioData.size(); ++i){
					output.write(reinterpret_cast<const char *>(&audioData[i]), sizeof(BitType));
				}
			}
			else{
				std::cout << "Not able to write to file" << std::endl;
				exit(0);
			}
		}
		
		//returns the size of file
		long filesize(const std::string &filename){
			std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
			return (long) in.tellg();
		}
		
		//concatenate two audio data together
		audio operator | (const audio & rhs){
			audio con(*this);
			con.audioDate.insert(con.audioData.end(), rhs.audioData.begin(), rhs.audioData.end());
			con.lenOfAudioSeconds = con.lenOfAudioSeconds + rhs.lenOfAudioSeconds;
			con.noOfSamples = con.noOfSamples + rhs.noOfSamples;
			return con;
		}		
	}
}
