add_rules("mode.debug")

target("maajsonviewer")
do
    add_rules("qt.widgetapp")
    set_kind("binary")
    set_languages("c++20")
    add_files("src/*.cpp")
    add_files("src/*.h") -- TODO: move qt stuff to src/ui

    after_build(function(target)
        local dstdir = path.join(target:targetdir())
        local src = path.join(path.directory(os.projectdir()), "MAA1999")
        local dst = path.join(dstdir, path.filename(src))
        if not os.exists(dst) then
            os.ln(path.relative(src, dstdir), dst)
        end
    end)
end
