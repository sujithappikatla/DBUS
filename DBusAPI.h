#ifndef DBusAPI_H
#define DBusAPI_H
#include <dbus/dbus.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

class DBusAPI
{
public:
	DBusAPI();
	~DBusAPI();
	bool create(char* busname,char* serverbusname=(char* )"");
	void send_message(char* param);
	void listener();
	bool callback(int x);
	bool callback(int x,int y);

private:

	DBusError dbus_error;
	DBusConnection *conn;
	char* SERVER_BUS_NAME;
	char* INTERFACE_NAME ;
	char* SERVER_OBJECT_PATH_NAME ;
	char* CLIENT_OBJECT_PATH_NAME ;
	char* METHOD_NAME ;
char* CLIENT_BUS_NAME;
int var;
};


#endif
