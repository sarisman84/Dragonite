function trymkdir(aDir)
    print(aDir)
    if not (os.isdir(aDir)) then
        os.mkdir(os.realpath(aDir))
    end
end