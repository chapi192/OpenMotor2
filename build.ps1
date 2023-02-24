cd $Args[0]
while (!(Get-ChildItem -Filter "CMakeLists.txt") -and !(Get-ChildItem -Filter "build.ps1")) {
	cd ..
}
if (Get-ChildItem -Filter "CMakeLists.txt") {
	if (!($Args[1] -eq "r")) {
		cmake -B build -G "MinGW Makefiles"
		cd build
		if ($LASTEXITCODE -eq 0) {
			mingw32-make.exe
		}
		echo ""
	} else {
		cd build
	}
	if ($LASTEXITCODE -eq 0 -or $Args[1] -eq "r") {
		echo "-- Start App"
		.\PhysSim.exe
		# .\projectile.exe
		echo ("-- End App, Exit Code: 0x" + '{0:X}' -f $LASTEXITCODE)
	}
	cd ..
} else {
	echo "failed to find a CMakeLists.txt"
}
