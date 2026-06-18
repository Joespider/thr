//#include <cstdio>
#include <iostream>
#include <string>
#include <unistd.h>
#include <stdexcept>
#include <stdio.h>
#include <thread>
#include <vector>
#include <fstream>
#include <filesystem>

//print Macro for cout
#define print(x); std::cout << x << std::endl

//error Macro for cerr
#define error(x); std::cerr << x << std::endl

//Convert std::string to String
#define String std::string

//Str for to_strin()
#define Str(x) std::to_string(x)

String TheName = "";

static void help();
bool StartsWith(String Str, String Start);
bool fexists(String aFile);
void copyFile(String Old, String New);
//void removeFile(String TheFile);
void shell(String threadNum, String RunShell, String ExeFlag, String command, bool BeQuiet, bool BeQuietTag, bool ToLog, bool ToLogWithThr);
String UseShell(String NewShell);
String ShellFlag(String NewShell);
int len(std::vector<String> Vect);

static void help()
{
	String Version = "0.1.13";
	print("Author: Joespider");
	print("Program: \"" << TheName << "\"");
	print("Version: " << Version);
	print("Purpose: \"run multiple commands at once\"");
	print("Usage: " << TheName << " <args>");
	print("{REQUIRED}");
	print("\t-s <shell> -c <command/script>\t\t\t: run commands or script");
	print("\t--shell <shell> --command <command/script>\t: run commands or script");
	print("\t-c <command/script>\t\t\t\t: run commands or script (defaulting to the \"sh\" shell)");
	print("\t--command <command/script>\t\t\t: run commands or script (defaulting to the \"sh\" shell)");
	print("");
	print("\t--sh <command/script>\t\t\t\t: sh shell run commands or script");
	print("\t--bash <command/script>\t\t\t\t: bash shell run commands or script");
	print("\t-ps <command/script>\t\t\t\t: PowerShell shell run commands or script");
	print("\t--pwsh <command/script>\t\t\t\t: PowerShell shell run commands or script");
	print("\t--powershell <command/script>\t\t\t: PowerShell shell run commands or script");
	print("\t-py <command/script>\t\t\t\t: python shell run commands or script");
	print("\t--python <command/script>\t\t\t: python shell run commands or script");
	print("\t-z <command/script>\t\t\t\t: zshell shell run commands or script");
	print("\t--zsh <command/script>\t\t\t\t: zshell run commands or script");
	print("");
	print("{OPTIONAL}");
	print("\t-q\t\t\t\t\t\t: hide output");
	print("\t--quiet\t\t\t\t\t\t: hide output");
	print("\t-qt\t\t\t\t\t\t: only show command output");
	print("\t--quiet-tag\t\t\t\t\t: only show command output");
	print("");
	print("\t--log\t\t\t\t\t\t: save output to a log (each thread is its own log file)");
	print("\t\t\t\t\t\t\t: -c \"ls\" would save output in \"ls.log\"");
	print("");
	print("\t--log-thr\t\t\t\t\t: save output to a log (each thread is its own log file)");
	print("\t\t\t\t\t\t\t: -c \"ls\" would save output in \"{" << TheName << "[<number>] \"ls\"}.log\"");
	print("");
	print("{EXAMPLE} Run three commands at once");
	print("\t$ " << TheName << " -s bash -c ls -c \"echo hi\" -c \"whoami\"");
	print("\t{" << TheName << "[2] /bin/bash -c \"echo hi\" (line: 0000)} hi");
	print("\t{" << TheName << "[1] /bin/bash -c \"ls\" (line: 0000)} Desktop");
	print("\t{" << TheName << "[1] /bin/bash -c \"ls\" (line: 0001)} Documents");
	print("\t{" << TheName << "[1] /bin/bash -c \"ls\" (line: 0002)} Downloads");
	print("\t{" << TheName << "[1] /bin/bash -c \"ls\" (line: 0003)} Music");
	print("\t{" << TheName << "[1] /bin/bash -c \"ls\" (line: 0004)} Pictures");
	print("\t{" << TheName << "[1] /bin/bash -c \"ls\" (line: 0005)} Public");
	print("\t{" << TheName << "[1] /bin/bash -c \"ls\" (line: 0006)} Videos");
	print("\t{" << TheName << "[3] /usr/bin/sh -c \"whoami\" (line: 0000)} user");
	print("");
	print("{EXAMPLE} To clean up the results in order of commands");
	print("\t$ " << TheName << " -c ls -c \"echo hi\" -c \"whoami\"  | sort | sed \"s/ (line: ....)//g\"");
	print("\t{" << TheName << "[1] /usr/bin/sh -c \"ls\"} Desktop");
	print("\t{" << TheName << "[1] /usr/bin/sh -c \"ls\"} Documents");
	print("\t{" << TheName << "[1] /usr/bin/sh -c \"ls\"} Downloads");
	print("\t{" << TheName << "[1] /usr/bin/sh -c \"ls\"} Music");
	print("\t{" << TheName << "[1] /usr/bin/sh -c \"ls\"} Pictures");
	print("\t{" << TheName << "[1] /usr/bin/sh -c \"ls\"} Public");
	print("\t{" << TheName << "[1] /usr/bin/sh -c \"ls\"} Videos");
	print("\t{" << TheName << "[2] /usr/bin/sh -c \"echo hi\"} hi");
	print("\t{" << TheName << "[3] /usr/bin/sh -c \"whoami\"} user");
	print("");
	print("\t$ " << TheName << " -s bash -c \"echo \"hi\"\" -s python -c \"print(\\\\\\\"hi\\\\\\\")\"");
	print("\t{" << TheName << "[1] /bin/bash -c \"echo \"hi\"\" (line: 0000)} hi");
	print("\t{" << TheName << "[2] /usr/bin/python3 -c \"print(\\\"hi\\\")\" (line: 0000)} hi\"");
	print("");
	print("\t$ " << TheName << " --bash \"echo \"hi\"\" -py \"print(\\\\\\\"hi\\\\\\\")\"");
	print("\t{" << TheName << "[1] /bin/bash -c \"echo \"hi\"\" (line: 0000)} hi");
	print("\t{" << TheName << "[2] /usr/bin/python3 -c \"print(\\\"hi\\\")\" (line: 0000)} hi\"");
}

//Check if string begins with substring
bool StartsWith(String Str, String Start)
{
	bool ItDoes = false;
	if (Str.rfind(Start, 0) == 0)
	{
		ItDoes = true;
	}
	return ItDoes;
}

int len(std::vector<String> Vect)
{
	int StrLen = Vect.size();
	return StrLen;
}

//check if file exists
bool fexists(String aFile)
{
	bool IsFound = false;
	std::ifstream ifile;
	ifile.open(aFile);
	if (ifile)
	{
		ifile.close();
		IsFound = true;
	}
	return IsFound;
}

void copyFile(String Old, String New)
{
	String data;
	std::ifstream read_file{ Old };
	std::ofstream save_file{ New };
	if (read_file && save_file)
	{
		while (std::getline(read_file, data))
		{
			save_file << data << "\n";
		}
	}
	//Close
	read_file.close();
	save_file.close();
}

/*
void removeFile(String TheFile)
{

	int passed = std::remove(TheFile);

	if (passed != 0)
	{
		error("could not remove \""+TheFile+"\"");
	}
}
*/

String UseShell(String NewShell)
{
	String ShellPath = "/usr/bin/sh";
	if (NewShell == "sh")
	{
		ShellPath = "/usr/bin/sh";
	}
	else if (NewShell == "bash")
	{
		ShellPath = "/bin/bash";
	}
	else if ((NewShell == "zsh") || (NewShell == "z"))
	{
		ShellPath = "/usr/bin/zsh";
	}
	else if (NewShell == "pwsh")
	{
		ShellPath = "/usr/bin/pwsh";
	}
	else if (NewShell == "python")
	{
		ShellPath = "/usr/bin/python3";
	}
/*
	else if (NewShell == "ruby")
	{
		ShellPath = "/usr/bin/ruby";
	}
*/
	else if ((StartsWith(NewShell, "/")) && (NewShell != "/"))
	{
		ShellPath = NewShell;
	}

	return ShellPath;
}

std::string ShellFlag(std::string NewShell)
{
	std::string ExeFlag = "";
	if ((NewShell == "/usr/bin/sh") || (NewShell == "/bin/bash") || (NewShell == "/usr/bin/zsh") || (NewShell == "/usr/bin/pwsh") || (NewShell == "/usr/bin/python3"))
	{
		ExeFlag = "-c";
	}
	else
	{
		ExeFlag = "";
	}

	return ExeFlag;
}


void shell(String threadNum, String RunShell, String ExeFlag, String command, bool BeQuiet, bool BeQuietTag, bool ToLog, bool ToLogWithThr)
{
	String FullCommand = "";

	if (ExeFlag != "")
	{
		FullCommand = RunShell+" "+ExeFlag+" \""+command+"\"";
	}
	else
	{
		FullCommand = RunShell+" \""+command+"\"";
	}

	if (fexists(RunShell))
	{
		std::ofstream myfile;
		char buffer[128];

		// Open pipe to file
		FILE* pipe = popen(FullCommand.c_str(), "r");
		if (!pipe)
		{
			error("popen failed!");
		}

		//Save command output to a log file
		if ((ToLog == true) || (ToLogWithThr == true))
		{
			String filename = "";
			if (ToLog == true)
			{
				//Generate File name for each thread
				filename = command+".log";
			}
			else if (ToLogWithThr == true)
			{
				//Generate File name for each thread
				filename = "thr["+threadNum+"] "+RunShell+" -c \""+command+"\".log";
			}
			myfile.open(filename.c_str());
		}

		long count = 0;
		// read till end of process:
		while (!feof(pipe))
		{
			// use buffer to read and add to result
			if (fgets(buffer, 128, pipe) != NULL)
			{
				if ((ToLog == true) || (ToLogWithThr == true))
				{
					myfile << buffer;
				}
				//suppress ALL output
				if (BeQuiet == false)
				{
					//suppress tags
					if (BeQuietTag == false)
					{
						if (count < 10)
						{
							std::cout << "{" << TheName << "["+threadNum+"] "+RunShell+" -c \""+command+"\" (line: 000" << count << ")} ";
						}
						else if ((count >= 10) && (count < 100))
						{
							std::cout << "{" << TheName << "["+threadNum+"] "+RunShell+" -c \""+command+"\" (line: 00" << count << ")} ";
						}
						else if ((count >= 100) && (count < 1000))
						{
							std::cout << "{" << TheName << "["+threadNum+"] "+RunShell+" -c \""+command+"\" (line: 0" << count << ")} ";
						}
						else
						{
							std::cout << "{" << TheName << "["+threadNum+"] "+RunShell+" -c \""+command+"\" (line: " << count << ")} ";
						}
					}
					std::cout << buffer;
					count++;
				}
			}
		}

		if (ToLog == true)
		{
			myfile.close();
		}

		pclose(pipe);
	}
	else
	{
		error("ERROR: \""+RunShell+"\" does not exist");
	}
}

//C++ Main...with cli arguments
int main(int argc, char** argv)
{
	std::vector<String> myShell;
	std::vector<String> myExeArg;
	std::vector<String> myCommands;
	std::vector<std::thread> myThreads;
	int numOfThreads;
	int numOfShells;
	String ThreadNum;
	String out = String(argv[0]);
	String ShellPath = "";
	String value = "";
	String PipeIn = "/dev/stdin";
	String PipeFile = "/tmp/thr.pipe";
	int next = 0;
	bool Quiet = false;
	bool QuietTags = false;
	bool SaveToLog = false;
	bool SaveToLogWithThr = false;
	bool IsNotOk = true;
	bool pipedData = false;
	bool InsertCmd = false;

	//piped in data is meant to be sent to commands
	if(!isatty(fileno(stdin)))
	{
		pipedData = true;

		if (fexists(PipeIn))
		{
			//copy file
			copyFile(PipeIn, PipeFile);

		}
	}

        //Parsing program name
        std::size_t pos = out.rfind('/');
	TheName = out.substr(pos + 1);

	out = "";
	//Args were given
	if (argc > 1)
	{
		//Loop through Args
		for (int i = 1; i < argc; i++)
		{
			out = String(argv[i]);
			if ((out == "-c") || (out == "--command"))
			{
				next = i+1;
				if (next < argc)
				{
					value = String(argv[next]);
					IsNotOk = StartsWith(value,"-");
					if (IsNotOk == false)
					{
						if (pipedData == true)
						{
							myCommands.push_back("cat "+PipeFile+" | "+value);
						}
						else
						{
							myCommands.push_back(value);
						}
					}
				}

				numOfThreads = len(myCommands);
				numOfShells = len(myShell);
				if (numOfThreads > numOfShells)
				{
					ShellPath = UseShell("sh");
					myShell.push_back(ShellPath);
					myExeArg.push_back(ShellFlag(ShellPath));
				}
			}
			else if (InsertCmd)
			{
				IsNotOk = StartsWith(out,"-");
				if (IsNotOk == false)
				{
					if (pipedData == true)
					{
						myCommands.push_back("cat "+PipeFile+" | "+out);
					}
					else
					{
						myCommands.push_back(out);
					}
				}
				InsertCmd = false;
			}
			else if ((out == "-s") || (out == "--shell"))
			{
				next = i+1;
				if (next < argc)
				{
					value = String(argv[next]);
                                        IsNotOk = StartsWith(value,"-");
                                        if (IsNotOk == false)
					{
						ShellPath = UseShell(value);
						myShell.push_back(ShellPath);
						myExeArg.push_back(ShellFlag(ShellPath));
					}
				}
			}
			else if (out == "--sh")
			{
				ShellPath = UseShell("sh");
				myShell.push_back(ShellPath);
				myExeArg.push_back(ShellFlag(ShellPath));
				InsertCmd = false;

				next = i+1;
				if (next < argc)
				{
					value = String(argv[next]);
                                        IsNotOk = StartsWith(value,"-");
                                        if (IsNotOk == false)
					{
						InsertCmd = true;
					}
				}
			}
			else if (out == "--bash")
			{
				ShellPath = UseShell("bash");
				myShell.push_back(ShellPath);
				myExeArg.push_back(ShellFlag(ShellPath));
				InsertCmd = false;

				next = i+1;
				if (next < argc)
				{
					value = String(argv[next]);
                                        IsNotOk = StartsWith(value,"-");
                                        if (IsNotOk == false)
					{
						InsertCmd = true;
					}
				}
			}
			else if ((out == "-ps") || (out == "--pwsh") || (out == "--powershell"))
			{
				ShellPath = UseShell("pwsh");
				myShell.push_back(ShellPath);
				myExeArg.push_back(ShellFlag(ShellPath));
				InsertCmd = false;

				next = i+1;
				if (next < argc)
				{
					value = String(argv[next]);
                                        IsNotOk = StartsWith(value,"-");
                                        if (IsNotOk == false)
					{
						InsertCmd = true;
					}
				}
			}
			else if ((out == "-py") || (out == "--python"))
			{
				ShellPath = UseShell("python");
				myShell.push_back(ShellPath);
				myExeArg.push_back(ShellFlag(ShellPath));
				InsertCmd = false;

				next = i+1;
				if (next < argc)
				{
					value = String(argv[next]);
                                        IsNotOk = StartsWith(value,"-");
                                        if (IsNotOk == false)
					{
						InsertCmd = true;
					}
				}
			}
/*
			else if ((out == "-rb") || (out == "--ruby"))
			{
				ShellPath = UseShell("ruby");
				myShell.push_back(ShellPath);
				myExeArg.push_back(ShellFlag(ShellPath));
				InsertCmd = false;

				next = i+1;
				if (next < argc)
				{
					value = String(argv[next]);
                                        IsNotOk = StartsWith(value,"-");
                                        if (IsNotOk == false)
					{
						InsertCmd = true;
					}
				}
			}
*/
			else if ((out == "-z") || (out == "--zsh"))
			{
				ShellPath = UseShell("zsh");
				myShell.push_back(ShellPath);
				myExeArg.push_back(ShellFlag(ShellPath));
				InsertCmd = false;

				next = i+1;
				if (next < argc)
				{
					value = String(argv[next]);
                                        IsNotOk = StartsWith(value,"-");
                                        if (IsNotOk == false)
					{
						InsertCmd = true;
					}
				}
			}
			else if (out == "--log")
			{
				SaveToLog = true;
				SaveToLogWithThr = false;
			}
			else if (out == "--log-thr")
			{
				SaveToLog = false;
				SaveToLogWithThr = true;
			}
			else if ((out == "-q") || (out == "--quiet"))
			{
				Quiet = true;
			}
			else if ((out == "-qt") || (out == "--quiet-tag"))
			{
				QuietTags = true;
			}
		}

		numOfThreads = len(myCommands);
		//Lets get our threads started
		if (numOfThreads > 0)
		{
			for (int lp = 0; lp != numOfThreads; lp++)
			{
				int ThrN = lp + 1;
				ThreadNum = Str(ThrN);
				std::thread ThreadName(shell,ThreadNum,myShell[lp],myExeArg[lp],myCommands[lp],Quiet,QuietTags,SaveToLog,SaveToLogWithThr);
				myThreads.push_back(std::move(ThreadName));
			}
			for (int lp = 0; lp != numOfThreads; lp++)
			{
				myThreads[lp].join();
			}

			if (pipedData == true)
			{
				int passed = std::remove("/tmp/thr.pipe");

				if (passed != 0)
				{
					error("could not remove \"/tmp/thr.pipe\"");
				}
			}
		}
		else
		{
			help();
		}
	}
	else
	{
		help();
	}

	return 0;
}
