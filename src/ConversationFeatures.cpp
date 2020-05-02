#include <sstream>
#include <iostream>
#include <iomanip>
#include "ConversationFeatures.h"
#include <cpp_redis/cpp_redis>
#include <tacopie/tacopie>
#include <stdio.h>
#include <string.h>
#include <sw/redis++/redis++.h>
using namespace sw::redis;

namespace FeatureExtractor {
	using namespace std;

	ConversationFeatures::ConversationFeatures(Conversation *conv)
		: conv(conv)
	{
		conv->register_reference();
	}


	ConversationFeatures::~ConversationFeatures()
	{
		// Conversation object commits suicide when nobody needs it anymore
		conv->deregister_reference();
	}


	Conversation *ConversationFeatures::get_conversation()
	{
		return conv;
	}

	/**
	 * Getters, setters, inc & dec for derived feature values
	 */
	uint32_t ConversationFeatures::get_count() const {
		return count;
	}
	void ConversationFeatures::set_count(uint32_t count) {
		this->count = count;
	}

	uint32_t ConversationFeatures::get_srv_count() const {
		return srv_count;
	}
	void ConversationFeatures::set_srv_count(uint32_t srv_count) {
		this->srv_count = srv_count;
	}

	double ConversationFeatures::get_serror_rate() const {
		return serror_rate;
	}
	void ConversationFeatures::set_serror_rate(double serror_rate) {
		this->serror_rate = serror_rate;
	}

	double ConversationFeatures::get_srv_serror_rate() const {
		return srv_serror_rate;
	}
	void ConversationFeatures::set_srv_serror_rate(double srv_serror_rate) {
		this->srv_serror_rate = srv_serror_rate;
	}

	double ConversationFeatures::get_rerror_rate() const {
		return rerror_rate;
	}
	void ConversationFeatures::set_rerror_rate(double rerror_rate) {
		this->rerror_rate = rerror_rate;
	}

	double ConversationFeatures::get_srv_rerror_rate() const {
		return srv_rerror_rate;
	}
	void ConversationFeatures::set_srv_rerror_rate(double srv_rerror_rate) {
		this->srv_rerror_rate = srv_rerror_rate;
	}

	double ConversationFeatures::get_same_srv_rate() const {
		return same_srv_rate;
	}
	void ConversationFeatures::set_same_srv_rate(double same_srv_rate) {
		this->same_srv_rate = same_srv_rate;
	}

	double ConversationFeatures::get_diff_srv_rate() const {
		return diff_srv_rate;
	}
	void ConversationFeatures::set_diff_srv_rate(double diff_srv_rate) {
		this->diff_srv_rate = diff_srv_rate;
	}

	double ConversationFeatures::get_srv_diff_host_rate() const {
		return (srv_count == 0) ? 0.0 : ((srv_count - same_srv_count) / (double)srv_count);
	}

	uint32_t ConversationFeatures::get_same_srv_count() const {
		return same_srv_count;
	}
	void ConversationFeatures::set_same_srv_count(uint32_t same_srv_count) {
		this->same_srv_count = same_srv_count;
	}

	uint32_t ConversationFeatures::get_dst_host_count() const {
		return dst_host_count;
	}
	void ConversationFeatures::set_dst_host_count(uint32_t dst_host_count) {
		this->dst_host_count = dst_host_count;
	}

	uint32_t ConversationFeatures::get_dst_host_srv_count() const {
		return dst_host_srv_count;
	}
	void ConversationFeatures::set_dst_host_srv_count(uint32_t dst_host_srv_count)	{
		this->dst_host_srv_count = dst_host_srv_count;
	}

	double ConversationFeatures::get_dst_host_same_srv_rate() const {
		return dst_host_same_srv_rate;
	}
	void ConversationFeatures::set_dst_host_same_srv_rate(double dst_host_same_srv_rate) {
		this->dst_host_same_srv_rate = dst_host_same_srv_rate;
	}

	double ConversationFeatures::get_dst_host_diff_srv_rate() const {
		return dst_host_diff_srv_rate;
	}
	void ConversationFeatures::set_dst_host_diff_srv_rate(double dst_host_diff_srv_rate) {
		this->dst_host_diff_srv_rate = dst_host_diff_srv_rate;
	}

	double ConversationFeatures::get_dst_host_same_src_port_rate() const {
		return dst_host_same_src_port_rate;
	}
	void ConversationFeatures::set_dst_host_same_src_port_rate(double dst_host_same_src_port_rate) {
		this->dst_host_same_src_port_rate = dst_host_same_src_port_rate;
	}

	double ConversationFeatures::get_dst_host_serror_rate() const {
		return dst_host_serror_rate;
	}
	void ConversationFeatures::set_dst_host_serror_rate(double dst_host_serror_rate) {
		this->dst_host_serror_rate = dst_host_serror_rate;
	}

	double ConversationFeatures::get_dst_host_srv_serror_rate() const {
		return dst_host_srv_serror_rate;
	}
	void ConversationFeatures::set_dst_host_srv_serror_rate(double dst_host_srv_serror_rate) {
		this->dst_host_srv_serror_rate = dst_host_srv_serror_rate;
	}

	double ConversationFeatures::get_dst_host_rerror_rate() const {
		return dst_host_rerror_rate;
	}
	void ConversationFeatures::set_dst_host_rerror_rate(double dst_host_rerror_rate) {
		this->dst_host_rerror_rate = dst_host_rerror_rate;
	}

	double ConversationFeatures::get_dst_host_srv_rerror_rate() const {
		return dst_host_srv_rerror_rate;
	}
	void ConversationFeatures::set_dst_host_srv_rerror_rate(double dst_host_srv_rerror_rate) {
		this->dst_host_srv_rerror_rate = dst_host_srv_rerror_rate;
	}

	double ConversationFeatures::get_dst_host_srv_diff_host_rate() const {
		return (dst_host_srv_count == 0) ? 0.0 : ((dst_host_srv_count - dst_host_same_srv_count) / (double)dst_host_srv_count);
	}

	uint32_t ConversationFeatures::get_dst_host_same_srv_count() const {
		return dst_host_same_srv_count;
	}
	void ConversationFeatures::set_dst_host_same_srv_count(uint32_t dst_host_same_srv_count) {
		this->dst_host_same_srv_count = dst_host_same_srv_count;
	}

	auto redis = Redis("tcp://127.0.0.1");
	// Allow using localtime instead of localtime_s 
	#pragma warning(disable : 4996)
	void ConversationFeatures::print(bool print_extra_features) const
	{
		stringstream ss;

		// Intrinsic features
		ss << noshowpoint << setprecision(0) << (conv->get_duration_ms() / 1000) << ','; // Cut fractional part
		//ss << conv->get_protocol_type_str() << ',';
		string protocols[3] = {"icmp" , "tcp", "udp"};
		string services[70]={"IRC", "X11", "Z39_50", "aol", "auth", "bgp", "courier", "csnet_ns", "ctf", 
"daytime", "discard", "domain", "domain_u", "echo", "eco_i", "ecr_i", "efs", 
"exec", "finger", "ftp", "ftp_data", "gopher", "harvest", "hostnames", "http", 
"http_2784", "http_443", "http_8001", "imap4", "iso_tsap", "klogin", "kshell",
 "ldap", "link", "login", "mtp", "name", "netbios_dgm", "netbios_ns", "netbios_ssn",
"netstat", "nnsp", "nntp", "ntp_u", "other", "pm_dump", "pop_2", "pop_3", "printer",
"private", "red_i", "remote_job", "rje", "shell", "smtp", "sql_net", "ssh", "sunrpc",
 "supdup", "systat", "telnet", "tftp_u", "tim_i", "time", "urh_i",
 "urp_i", "uucp", "uucp_path", "vmnet", "whois"};
		string flags[11]={"flag_OTH", "flag_REJ", "flag_RSTO", "flag_RSTOS0", "flag_RSTR", "flag_S0",
		 "flag_S1", "flag_S2", "flag_S3", "flag_SF", "flag_SH"};
    // Print Strings 
    	for (int i = 0; i < 3; i++){
		
			if(strcmp(conv->get_protocol_type_str(),protocols[i].c_str())==0){
				cout << protocols[i] << "\n";
				cout << conv->get_protocol_type_str() << "\n"; 
				ss << "1" << ',';
			}else{
				ss << "0" << ',';
			}	
		}

		for (int i = 0; i < 70; i++){
		
			if(strcmp(conv->get_service_str(),services[i].c_str())==0){
				cout << services[i] << "\n";
				cout << conv->get_protocol_type_str() << "\n"; 
				ss << "1" << ',';
			}else{
				ss << "0" << ',';
			}	
		}

			for (int i = 0; i < 11; i++){
		
			if(strcmp(conv->get_state_str(),flags[i].c_str())==0){
				cout << flags[i] << "\n";
				cout << conv->get_protocol_type_str() << "\n"; 
				ss << "1" << ',';
			}else{
				ss << "0" << ',';
			}	
		}


        
		//ss << conv->get_service_str() << ',';
		//if(strcmp(conv->get_service_str(),""))
		//ss << conv->get_state_str() << ',';
		ss << conv->get_src_bytes() << ',';
		ss << conv->get_dst_bytes() << ',';
		ss << conv->land() << ',';
		ss << conv->get_wrong_fragments() << ',';
		ss << conv->get_urgent_packets() << ',';

		// Derived time windows features
		ss << fixed << showpoint <<setprecision(2);
		ss << count << ',';
		ss << srv_count << ',';
		ss << serror_rate << ',';
		ss << srv_serror_rate << ',';
		ss << rerror_rate << ',';
		ss << srv_rerror_rate << ',';
		ss << same_srv_rate << ',';
		ss << diff_srv_rate << ',';
		ss << get_srv_diff_host_rate() << ',';

		// Derived connection count window features
		ss << dst_host_count << ',';
		ss << dst_host_srv_count << ',';
		ss << dst_host_same_srv_rate << ',';
		ss << dst_host_diff_srv_rate << ',';
		ss << dst_host_same_src_port_rate << ',';
		ss << get_dst_host_srv_diff_host_rate() << ',';
		ss << dst_host_serror_rate << ',';
		ss << dst_host_srv_serror_rate << ',';
		ss << dst_host_rerror_rate << ',';
		ss << dst_host_srv_rerror_rate;

		if (true) {
			const FiveTuple *ft = conv->get_five_tuple_ptr();

			// TODO: ugly wtf, but working
			uint32_t src_ip = ft->get_src_ip();
			uint32_t dst_ip = ft->get_dst_ip();
			uint8_t *sip = (uint8_t *)&src_ip;
			uint8_t *dip = (uint8_t *)&dst_ip;
			ss << ',';
			ss << (int)sip[0] << "." << (int)sip[1] << "." << (int)sip[2] << "." << (int)sip[3] << ';';
			ss << ft->get_src_port() << ';';
			ss << (int)dip[0] << "." << (int)dip[1] << "." << (int)dip[2] << "." << (int)dip[3] << ';';
			ss << ft->get_dst_port() << ';';

			// Time (e.g.: 2010-06-14T00:11:23)
			struct tm *ltime;
			//struct tm timeinfo;
			char timestr[20];
			time_t local_tv_sec;
			local_tv_sec = conv->get_last_ts().get_secs();
			ltime = localtime(&local_tv_sec);
			//localtime_s(&timeinfo, &local_tv_sec);
			strftime(timestr, sizeof timestr, "%Y-%m-%dT%H:%M:%S", ltime);
			//strftime(timestr, sizeof timestr, "%Y-%m-%dT%H:%M:%S", &timeinfo);
			ss << timestr;
				

	// save to a redis datbase
		}
		struct tm *ltime;
			//struct tm timeinfo;
			char timestr[20];
			time_t local_tv_sec;
			local_tv_sec = conv->get_last_ts().get_secs();
			ltime = localtime(&local_tv_sec);
			//localtime_s(&timeinfo, &local_tv_sec);
			strftime(timestr, sizeof timestr, "%Y-%m-%dT%H:%M:%S", ltime);
			//strftime(timestr, sizeof timestr, "%Y-%m-%dT%H:%M:%S", &timeinfo);
		//	ss << timestr;
		

		string rediskey;
		string str(timestr);

		// string hash
		std::hash<std::string> str_hash;

		// generate redis key
		rediskey = str + "-" + to_string(str_hash(ss.str()));

		
		// synchronous commit, no timeout
		
			

	using Attrs = std::vector<std::pair<std::string, std::string>>;

	// You can also use std::unordered_map, if you don't care the order of attributes:
	// using Attrs = std::unordered_map<std::string, std::string>;

	Attrs features = { {rediskey, ss.str()}, {"f2", "v2"} };

	// Adding an feature string to the redis stream
	cout << " REDIS to STREAM" << endl;
	auto id = redis.xadd("networkStream", "*", features.begin(), features.end());
	
		cout << ss.str() << endl;
	}
	

	void ConversationFeatures::print_human() const
	{
		conv->print_human();

		stringstream ss;
		ss << fixed << setprecision(2);
		ss << "count = " << count << endl;
		ss << "srv_count = " << srv_count << endl;
		ss << "serror_rate = " << serror_rate << endl;
		ss << "srv_serror_rate = " << srv_serror_rate << endl;
		ss << "rerror_rate = " << rerror_rate << endl;
		ss << "srv_rerror_rate = " << srv_rerror_rate << endl;
		ss << "same_srv_rate = " << same_srv_rate << endl;
		ss << "diff_srv_rate = " << diff_srv_rate << endl;
		ss << "get_srv_diff_host_rate = " << get_srv_diff_host_rate() << endl;
		ss << "dst_host_count = " << dst_host_count << endl;
		ss << "dst_host_srv_count = " << dst_host_srv_count << endl;
		ss << "dst_host_same_srv_rate = " << dst_host_same_srv_rate << endl;
		ss << "dst_host_diff_srv_rate = " << dst_host_diff_srv_rate << endl;
		ss << "dst_host_same_src_port_rate = " << dst_host_same_src_port_rate << endl;
		ss << "get_dst_host_srv_diff_host_rate = " << get_dst_host_srv_diff_host_rate() << endl;
		ss << "dst_host_serror_rate = " << dst_host_serror_rate << endl;
		ss << "dst_host_srv_serror_rate = " << dst_host_srv_serror_rate << endl;
		ss << "dst_host_rerror_rate = " << dst_host_rerror_rate << endl;
		ss << "dst_host_srv_rerror_rate = " << dst_host_srv_rerror_rate << endl;
		// Time (e.g.: 2010-06-14T00:11:23)
			struct tm *ltime;
			//struct tm timeinfo;
			char timestr[20];
			time_t local_tv_sec;
			local_tv_sec = conv->get_last_ts().get_secs();
			ltime = localtime(&local_tv_sec);
			//localtime_s(&timeinfo, &local_tv_sec);
			strftime(timestr, sizeof timestr, "%Y-%m-%dT%H:%M:%S", ltime);
			//strftime(timestr, sizeof timestr, "%Y-%m-%dT%H:%M:%S", &timeinfo);
			ss << timestr;
		cout << ss.str() << endl;
		
		cpp_redis::client client;
		if (true)
	{
		client.connect("127.0.0.1", 6379, [](const std::string &host, std::size_t port, cpp_redis::client::connect_state status) {
			if (status == cpp_redis::client::connect_state::dropped)
			{
				std::cout << "client disconnected from " << host << ":" << port << std::endl;
			}
		});


		string rediskey;
		string str(timestr);

		// string hash
		std::hash<std::string> str_hash;

		// generate redis key
		rediskey = str + "-" + to_string(str_hash(ss.str()));

		client.set(rediskey, ss.str(), [](cpp_redis::reply &reply) {
			
		});
		// synchronous commit, no timeout
		client.sync_commit();
	}


	}

}
