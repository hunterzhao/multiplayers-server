#ifndef PARSEMESSAGE
#define PARSEMESSAGE
#include <sstream>
#include <vector>
#include <string>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "cmd.h"

using namespace rapidjson;
using Message = Document; 
namespace Game {
std::vector<Cmd> str2Json(const char* msg, int len)
{
   std::vector<Cmd> ans;
   std::string str(msg, len);
   std::stringstream ss;
   ss << str;
   std::string jsonstr;

   while(std::getline(ss, jsonstr, '#')) {
       Message d;
       if (d.Parse(jsonstr.c_str()).HasParseError()) {
       	   printf("invalid jsonstr");
       	   continue;
       }
       ans.push_back(Cmd(d["id"].GetInt(), d["x"].GetDouble(), 
       	                 d["z"].GetDouble(), d["type"].GetInt()));
   }
   return ans;
}

std::string json2Str(std::vector<Cmd> vec)
{
   std::string ans = "";
   
   StringBuffer buffer;
   Writer<StringBuffer> writer(buffer);
   for (Cmd c : vec) {
       StringBuffer s;
       Writer<StringBuffer> writer(s);
       writer.StartObject(); 

        writer.Key("id");
        writer.Uint(c.id); 
        writer.Key("x");
        writer.Double(c.x);
        writer.Key("z");
        writer.Double(c.z);
        writer.Key("type");
        writer.Uint(c.type);
        writer.EndObject();
       ans += s.GetString();
       ans +='#';
   }
   return ans;
}
}
#endif