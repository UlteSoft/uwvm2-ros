// Independent selected Host canonical decoder; not a native TF middleware peer.
#include "src/uwvm2/imported/ros/ipc/transform.h"
#include <fstream>
#include <iostream>
#include <stdexcept>

int main(int argc,char** argv) {
 try {
  if(argc!=2)throw std::runtime_error("one corpus path required");
  std::ifstream file(argv[1]);if(!file)throw std::runtime_error("corpus open");
  std::string line;unsigned index{};
  while(std::getline(file,line)) {
   if(index>=4096||line.size()>4096||line.size()<2||line[1]!=' ')throw std::runtime_error("corpus bounds");
   unsigned backend=unsigned(line[0]-'0');std::vector<std::uint8_t> bytes;
   if((line.size()-2)%2)throw std::runtime_error("hex extent");
   auto nibble=[](char c)->unsigned { if(c>='0'&&c<='9')return unsigned(c-'0');if(c>='a'&&c<='f')return unsigned(c-'a'+10);throw std::runtime_error("hex"); };
   for(std::size_t n=2;n<line.size();n+=2)bytes.push_back(std::uint8_t(nibble(line[n])*16+nibble(line[n+1])));
   uwvm2::imported::ros::ipc::transform_data value;
   bool valid=(backend==1||backend==2)&&uwvm2::imported::ros::ipc::decode_transform(bytes,value)&&(backend!=2||!value.sequence);
   std::vector<std::uint8_t> encoded;if(valid)encoded=uwvm2::imported::ros::ipc::encode_transform(value);
   std::cout<<"TRANSFORM_CASE "<<index<<' '<<backend<<' '<<(valid?0:22)<<' '<<encoded.size()<<'\n';
   for(std::size_t n=0;n<encoded.size();++n)std::cout<<"TRANSFORM_BYTE "<<index<<' '<<n<<' '<<unsigned(encoded[n])<<'\n';
   ++index;
  }
  if(!file.eof()||!index)throw std::runtime_error("corpus input");
  std::cout<<"PASS_MOONBIT_TRANSFORM_CORPUS "<<index<<'\n';
  return 0;
 }catch(std::exception const& e){std::cerr<<e.what()<<'\n';return 1;}
}
