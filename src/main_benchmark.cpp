﻿////////////////////////////////////////////////////////////////////////////////
//
// Game NET - A Simple Network Tutorial for OpenGL Games
//
// (C) by Sven Forstmann in 2015
//
// License : MIT
// http://opensource.org/licenses/MIT
////////////////////////////////////////////////////////////////////////////////
#include "core.h"
#include "net_rpc.h"
#include "net_client.h"
#include "net_server.h"

////////////////////////////////////////////////////////////////////////////////
// Simple Hello World
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Server Part

NetServer server(12345,0);

// RPC
void hello_server(uint clientid, string s)
{
	//cout << "Client " << clientid << " sends " << s << endl;
	//server.call(clientid, "hello_client",  "Greetings from Server");

	static uint t = core_time() % 1000;
	uint		t_now = core_time()%1000;
	static uint bench = 0; bench=bench+1;	

	if (t_now<t)
	{
		cout << bench << " RPCs/s " << endl;
		bench = 0;
	}
	t = t_now;
}

// Main
void start_server()
{
	Rpc &r = server.get_rpc();
	rpc_register_remote(r, hello_client);
	rpc_register_local (r, hello_server);
	server.start();
	//server.stop();
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Client Part

NetClient client;

// Client RPCs
void hello_client(string s)
{
	//cout << "Server sends " << s << endl;
};

// Main
void start_client()
{
	core_sleep(1000);
	Rpc &r = client.get_rpc();
	rpc_register_local (r, hello_client);
	rpc_register_remote(r, hello_server);
	client.connect("localhost", 12345);
	
	uint c;

	while (client.connected())
	{
		loopi(0, 20) client.call("hello_server", "Greetings");

		client.process();
	}
	//client.disconnect();
	core_sleep(1000);
	server.stop();
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Main
int main()
{
	start_server();
	start_client();
	return 0;
}
////////////////////////////////////////////////////////////////////////////////
