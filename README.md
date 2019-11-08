

push v1 - Coded all of this today. Been going at it for a few hours. Added all necessary functions I need so far.
          Program seems to be working fine. Unsure on if processes should reawaken, so as of right now, there are always 18 processes running.
          I think the output is correct, but I am unsure. 
          Have not added verbose, or a waiting if a process is denied allocation for said process to free resources.\n
push v2 - Formatted the printed table a bit better. Verified that only 18 processes ever open.
          Commented out the ability to revive children. Haven't added verbose yet, that's next.
          Added display for the program's stats.\n
push v3 - Had to completely change the project. Was working with changing one resource, had to modify to alter 20 per process.
          That being said, I had 3 arrays in the struct prior, now I only needed 2, which was a max claims, and another is the resources.
          Updated the output, looks gorgeous now. The max resources array is hard coded, may make that randomized.
          Still need to add a queue for resources that are denied requests due to memory not being free.\n
