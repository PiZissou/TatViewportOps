/*
<grimoire>
	<description>C++ project who extends viewport methods</description>
	<authors>PLA</authors>
	<version> 0.0.1 </version>
	<categories>General</categories>
	<tags>Viewport, Guide, Camera</tags>
</grimoire>
*/

version = substring (GetFileVersion (getDir #maxRoot + "\\3dsmax.exe")) 1 2
dlcFile = ""
if version == "19" then (
    dlcFile = ""
) else if version == "22" then (
    dlcFile = ""
) else if version == "23" then (
    dlcFile = "TatViewportOps.dlu"
)


dlcPath = getFilenamePath(getThisScriptFilename())

localPath = GetDir #temp + "\\"

localFile = localPath + "TatViewportOps.dlu"
deleteFile localFile

if dlcFile != "" then (
	networkFile = dlcPath + dlcFile

	copyFile networkFile localFile

	LoadDllsFromDir   localPath  "TatViewportOps.dlu"
)

