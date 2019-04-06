tar_ball=$$basename(UPSTREAM_URL)
ex_dir=$$replace(tar_ball, .tar.gz,)
update_script=$$PRO_DIR/update-$${TARGET}.sh

!exists($$SRC_DIR){
    QMAKE_DEBUG: message($$SRC_DIR)
    QMAKE_DEBUG: message($$TARGET source folder not found. Will try to download it on qmake )
    QMAKE_DEBUG: message(upstream: $$UPSTREAM_URL )
    QMAKE_DEBUG:  message(looking for $$update_script)
    exists($$update_script){
        QMAKE_DEBUG: message( $$TARGET Has override update script )
        system($$update_script)
    } else {
        message( WARN: no update script found gonna try our best )
        COMMAND =  "cd $$PRO_DIR;"
        COMMAND += "curl -OL $$UPSTREAM_URL;"
        COMMAND += "tar -zxf $$tar_ball;"
        COMMAND += "cp -r $$ex_dir $$TARGET;"
        COMMAND += "rm -rf $$tar_ball $$ex_dir;"
        COMMAND += "cd -;"
        system($$COMMAND)
    }
} else {
    QMAKE_DEBUG: message( $$TARGET source is on disk. to remove run make distclean )
}

#QMAKE_DISTCLEAN += -r $$SRC_DIR
