[Setup]
AppId={{B977297C-595E-44F1-B173-727E2C43999F}
AppName=Process Killer
AppVersion=1.1
;AppVerName=Process Killer 1.1
AppPublisher=Explyne
DefaultDirName={autopf}\Process Killer
DefaultGroupName=Process Killer
AllowNoIcons=yes
PrivilegesRequired=admin
OutputDir=D:\ProcessKiller_arch\InnoSetup_Script\Output\
OutputBaseFilename=ProcessKiller setup
SetupIconFile=D:\ProcessKiller_arch\InnoSetup_Script\ProcessKillerIcon.ico
WizardSmallImageFile=D:\ProcessKiller_arch\InnoSetup_Script\smallWizardicon.bmp
WizardImageFile=D:\ProcessKiller_arch\InnoSetup_Script\bigWizardicon.bmp
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
Source: "D:\ProcessKiller_arch\ProcessKiller.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\ProcessKiller_arch\propertions.ini"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{group}\Process Killer"; Filename: "{app}\ProcessKiller.exe"
Name: "{group}\{cm:UninstallProgram,Process Killer}"; Filename: "{uninstallexe}"
Name: "{autodesktop}\Process Killer"; Filename: "{app}\ProcessKiller.exe"; Tasks: desktopicon

[Run]
Filename: "{app}\ProcessKiller.exe"; Description: "{cm:LaunchProgram,Process Killer}"; Flags: nowait postinstall skipifsilent
