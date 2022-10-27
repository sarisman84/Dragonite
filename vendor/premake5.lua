function dir_exists(dir)
    local isok, errstr, errcode = os.rename(dir, dir)
    if isok == nil then
        if errcode == 13 then
            -- Permission denied, but it exists
            return true
        end
        return false
    end
    return true
end

-- split("a,b,c", ",") => {"a", "b", "c"}
function split(s, sep)
    local fields = {}

    local sep = sep or " "
    local pattern = string.format("([^%s]+)", sep)
    string.gsub(s, pattern, function(c)
        fields[#fields + 1] = c
    end)

    return fields
end

function fetchVendorInclude(aVendorName, aWorkingDir)

    aWorkingDir = os.realpath(aWorkingDir)
    printf(aWorkingDir)
    local r = os.isdir(aWorkingDir)
    if (r) then
        printf("Work Dir is valid")
    else
        printf("Work Dir is not valid")
        os.mkdir(aWorkingDir)
    end
    -- local curDir = string.gsub(debug.getinfo(1).short_src, "^(.+\\)[^\\]+$", "%1")
    -- curDir = string.gsub(curDir, "vendor/premake5.lua", "")
    -- local localDir = aWorkingDir .. "/vendor/"
    -- local dir = curDir .. localDir
    -- printf(dir)

    -- local dirs = split(localDir, "/")
    -- local r = os.execute("{CHDIR} ..")
    -- r = os.execute("{CHDIR} " .. aWorkingDir)
    -- if (r == false) then
    --     printf("failed to move command console")
    -- end
    -- for d in pairs(dirs) do
    --     os.execute("{MKDIR} " .. dirs[d])
    --     os.execute("{CHDIR} " .. dirs[d])
    --     printf(dirs[d])
    -- end
    -- local targetDir = localDir .. aVendorName
    -- local vendorDir = curDir .. aVendorName

    -- os.execute("{COPYDIR} " .. vendorDir .. " " .. targetDir)

    -- -- Cpy it to the aWorkingDir

    -- printf("Target Dir: " .. targetDir)
    -- return its dir as a result
    return ""
end

