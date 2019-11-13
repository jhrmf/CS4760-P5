# P5
Assignment 5
CS4760 Fall Semester
Joseph Hill
Repository - https://github.com/jhrmf/P5

Invocation:
        ./oss [-h] [-v]

Project Description
        Create a project that manages multiple resources per process in a simulation, along with deadlock avoidance.
        METHOD TO COMMUNICATE TO OS FROM USER - shared memory

Arguments
    -h : Display a help message
    -v : Set verbose to off, it will only display denied requests to avoid deadlock.

Version Control
push v1 - Coded all of this today. Been going at it for a few hours. Added all necessary functions I need so far.
          Program seems to be working fine. Unsure on if processes should reawaken, so as of right now, there are always 18 processes running.
          I think the output is correct, but I am unsure. 
          Have not added verbose, or a waiting if a process is denied allocation for said process to free resources.
push v2 - Formatted the printed table a bit better. Verified that only 18 processes ever open.
          Commented out the ability to revive children. Haven't added verbose yet, that's next.
          Added display for the program's stats.
push v3 - Had to completely change the project. Was working with changing one resource, had to modify to alter 20 per process.
          That being said, I had 3 arrays in the struct prior, now I only needed 2, which was a max claims, and another is the resources.
          Updated the output, looks gorgeous now. The max resources array is hard coded, may make that randomized.
          Still need to add a queue for resources that are denied requests due to memory not being free.
push v4 - I messed around in a temporary file I made for this project and sadly cannot get the code to work for a queue and deadlock avoidance with said queue.
          as the project stands now it will never deadlock, rendering verbose almost useless. Otherwise, the project works perfectly.

Notes: When the server is bogged down the program has trouble completing its run. Otherwise, works swimmingly.

Any code that is not my own was either approved by a professor, provided by a professor, or from Linux Man Pages.

Thanks!