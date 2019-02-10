#include "DBusAPI.h"



DBusAPI::DBusAPI()
{
	INTERFACE_NAME = (char* )"in.softprayog.dbus_example";
	SERVER_OBJECT_PATH_NAME = (char* )"/in/softprayog/adder";
	CLIENT_OBJECT_PATH_NAME = (char* )"/in/softprayog/add_client";
	METHOD_NAME = (char* )"add_numbers";
var=5;
	printf("oobject path : %s",SERVER_OBJECT_PATH_NAME);
}


bool DBusAPI::create(char* busname,char* serverbusname){
	
    int ret;
	
	SERVER_BUS_NAME=serverbusname;
CLIENT_BUS_NAME=busname;
	
    dbus_error_init (&dbus_error);

    conn = dbus_bus_get (DBUS_BUS_SYSTEM, &dbus_error);

    if (dbus_error_is_set (&dbus_error))
	{
		fprintf (stderr, "dbus_bus_get: %s\n", dbus_error.message);
		dbus_error_free (&dbus_error);
	}

    if (!conn){
		
		 exit (1);
	} 
       

    // Get a well known name
    ret = dbus_bus_request_name (conn, busname, DBUS_NAME_FLAG_DO_NOT_QUEUE, &dbus_error);

    if (dbus_error_is_set (&dbus_error))
    {
		fprintf (stderr, "dbus_bus_get: %s\n", dbus_error.message);
		dbus_error_free (&dbus_error);
	}

    if (ret != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER) {
        fprintf (stderr, "Dbus: not primary owner, ret = %d\n", ret);
        exit (1);
    }
		return true;
}


void DBusAPI::send_message(char* param)
{
	DBusMessage *request;


        if ((request = dbus_message_new_method_call (SERVER_BUS_NAME, SERVER_OBJECT_PATH_NAME, INTERFACE_NAME, METHOD_NAME)) == NULL) {
            fprintf (stderr, "Error in dbus_message_new_method_call\n");
            exit (1);
        }

        DBusMessageIter iter;
        dbus_message_iter_init_append (request, &iter);
        
        if (!dbus_message_iter_append_basic (&iter, DBUS_TYPE_STRING, &param)) {
            fprintf (stderr, "Error in dbus_message_iter_append_basic\n");
            exit (1);
        }
        DBusPendingCall *pending_return;
        if (!dbus_connection_send_with_reply (conn, request, &pending_return, -1)) {
            fprintf (stderr, "Error in dbus_connection_send_with_reply\n");
            exit (1);
        }

        if (pending_return == NULL) {
            fprintf (stderr, "pending return is NULL");
            exit (1);
        }

        dbus_connection_flush (conn);
                
        dbus_message_unref (request);	

        dbus_pending_call_block (pending_return);

        DBusMessage *reply;
        if ((reply = dbus_pending_call_steal_reply (pending_return)) == NULL) {
            fprintf (stderr, "Error in dbus_pending_call_steal_reply");
            exit (1);
        }

        dbus_pending_call_unref	(pending_return);

        char *s;
        if (dbus_message_get_args (reply, &dbus_error, DBUS_TYPE_STRING, &s, DBUS_TYPE_INVALID)) {
            printf ("%s\n", s);
        }
        else
        {
             fprintf (stderr, "Did not get arguments in reply\n");
             exit (1);
        }
        dbus_message_unref (reply);	

        if (dbus_bus_release_name (conn, CLIENT_BUS_NAME, &dbus_error) == -1) {
             fprintf (stderr, "Error in dbus_bus_release_name\n");
             exit (1);
        }
	
}




void DBusAPI::listener()
{
	while (1) {
        // Block for msg from client
        if (!dbus_connection_read_write_dispatch (conn, -1)) {
            fprintf (stderr, "Not connected now.\n");
            exit (1);
        }
     	printf("block");
        DBusMessage *message;

        if ((message = dbus_connection_pop_message (conn)) == NULL) {
            fprintf (stderr, "Did not get message\n");
            continue;
        }
		if (dbus_message_is_method_call (message, INTERFACE_NAME, METHOD_NAME)) {
            char *s;
            
            if (dbus_message_get_args (message, &dbus_error, DBUS_TYPE_STRING, &s, DBUS_TYPE_INVALID)) {
                printf (" string : %s \n", s);
                char* one=(char* )"one";
		char* two =(char* )"two";
               		/*if(strcmp(s,one)==0){
				this->callback(5);
			}
			if(strcmp(s,two)==0){
				this->callback(5,8);
			}*/
			if(callback(888)){
				DBusMessage *reply;
                    char answer [40];

                    sprintf (answer, "Sum is something" );
                    if ((reply = dbus_message_new_method_return (message)) == NULL) {
                        fprintf (stderr, "Error in dbus_message_new_method_return\n");
                        exit (1);
                    }
    
                    DBusMessageIter iter;
                    dbus_message_iter_init_append (reply, &iter);
                    char *ptr = answer;
                    if (!dbus_message_iter_append_basic (&iter, DBUS_TYPE_STRING, &ptr)) {
                        fprintf (stderr, "Error in dbus_message_iter_append_basic\n");
                        exit (1);
                    }

                    if (!dbus_connection_send (conn, reply, NULL)) {
                        fprintf (stderr, "Error in dbus_connection_send\n");
                        exit (1);
                    }

                    dbus_connection_flush (conn);
                
                    dbus_message_unref (reply);	


			}

                    // send reply
                   /* DBusMessage *reply;
                    char answer [40];

                    sprintf (answer, "Sum is something" );
                    if ((reply = dbus_message_new_method_return (message)) == NULL) {
                        fprintf (stderr, "Error in dbus_message_new_method_return\n");
                        exit (1);
                    }
    
                    DBusMessageIter iter;
                    dbus_message_iter_init_append (reply, &iter);
                    char *ptr = answer;
                    if (!dbus_message_iter_append_basic (&iter, DBUS_TYPE_STRING, &ptr)) {
                        fprintf (stderr, "Error in dbus_message_iter_append_basic\n");
                        exit (1);
                    }

                    if (!dbus_connection_send (conn, reply, NULL)) {
                        fprintf (stderr, "Error in dbus_connection_send\n");
                        exit (1);
                    }

                    dbus_connection_flush (conn);
                
                    dbus_message_unref (reply);	
                
                */
            }
            else
            {
				
				fprintf (stderr, "Error getting message: %s\n", dbus_error.message);
				dbus_error_free (&dbus_error);
	
            }
        }
    }
}

bool DBusAPI::callback(int x)
{
	printf("callback one %d",x);
return true;
}

bool DBusAPI::callback(int x,int  y)
{
	printf("callback two %d %d",x,y);
return true;
}

