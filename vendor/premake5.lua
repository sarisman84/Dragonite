
function copyDir(aSource, aDestination)
    os.execute("xcopy /S /Y " .. aSource .. " " .. aDestination)
    return aDestination
end

function fetchVendorInclude(aVendorName, aWorkingDir)

    local finalVendorDir
    aWorkingDir = os.realpath(aWorkingDir .. "/vendor/" .. aVendorName)
    local vendor = os.realpath("../vendor/" .. aVendorName)
    printf("Working Dir: " .. aWorkingDir)
    local r = os.isdir(aWorkingDir)
    if (r) then
        printf("Work Dir is valid")
    else
        printf("Work Dir is not valid")
        os.mkdir(aWorkingDir)
    end

    printf("Vendor: " .. vendor)
    r = os.isdir(vendor);
    if (r) then
        printf("Found Vendor")
        finalVendorDir = copyDir(vendor, aWorkingDir)
    else
        printf("Could not find vendor")
    end
    return finalVendorDir
end

