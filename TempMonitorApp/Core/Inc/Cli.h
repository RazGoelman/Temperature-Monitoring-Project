/*
 * Cli.h
 *

 */

#ifndef INC_CLI_H_
#define INC_CLI_H_

#define COMMAND_ARR_SIZE 10
#define COMMAND_NAME_SIZE 20

#include <stdio.h>
#include "main.h"
#include <string.h>
#include "Flash.h"

class Cli{
public:
	virtual void doCommand(const char * param) = 0;

	virtual ~Cli(){}
};

class clicommand_entry {
public:
	const char * CommandName;
	Cli* command;
};

class CliContainer{
private:
	clicommand_entry commands[20];
	int funcounter = 0;
public:
	CliContainer(){}
	~CliContainer(){}
	void RegisterCommand(const  char* CommandName, Cli * clicommand) {
		commands[funcounter].CommandName = CommandName;
		commands[funcounter].command = clicommand;
		funcounter++;
	}

	void doCommand( char * cmd,  char * param){
		for (int j = 0; j <funcounter; j++) {
			if (strcmp(cmd,commands[j].CommandName) == 0) {
				commands[j].command->doCommand(param);
				return;
			}

		}
		printf("Invalid command\r\n");
	}
	void initCLIcontainer();
};

class SetWarning : public Cli
{
private:
	Flash* _flash;
public:
	void doCommand(char* param);
};

class SetCritical : public Cli
{
private:
	Flash* _flash;
public:
	void doCommand(char* param);
};
class GetFlashInfo : public Cli
{
private:
	Flash* _flash;
public:
	void doCommand(char* param);

};
#endif /* INC_CLI_H_ */
