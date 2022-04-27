; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

[Setup]
; NOTE: The value of AppId uniquely identifies this application. Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{B977297C-595E-44F1-B173-727E2C43999F}
AppName=Process Killer
AppVersion=1.0
;AppVerName=Process Killer 1.0
AppPublisher=Explyne
DefaultDirName={autopf}\Process Killer
DefaultGroupName=Process Killer
AllowNoIcons=yes
; Uncomment the following line to run in non administrative install mode (install for current user only.)
PrivilegesRequired=admin
OutputDir=D:\
OutputBaseFilename=ProcessKiller setup
SetupIconFile=C:\Users\Explyne\Downloads\unnamed.ico
Compression=lzma
SolidCompression=yes
WizardStyle=modern

[Registry]
Root: HKCU; Subkey: "SOFTWARE\Microsoft\Windows\CurrentVersion\Run"; ValueType: string; ValueName: "Process Killer"; ValueData: """{app}\ProcessKiller.exe"""; Flags: uninsdeletevalue

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "C:\Users\Explyne\source\repos\ProcessKiller\x64\Debug\ProcessKiller.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Users\Explyne\source\repos\ProcessKiller\x64\Debug\propertions.ini"; DestDir: "{app}"; Flags: ignoreversion
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\Process Killer"; Filename: "{app}\ProcessKiller.exe"
Name: "{group}\{cm:UninstallProgram,Process Killer}"; Filename: "{uninstallexe}"
Name: "{autodesktop}\Process Killer"; Filename: "{app}\ProcessKiller.exe"; Tasks: desktopicon

[Run]
Filename: "{app}\ProcessKiller.exe"; Description: "{cm:LaunchProgram,Process Killer}"; Flags: nowait postinstall skipifsilent
