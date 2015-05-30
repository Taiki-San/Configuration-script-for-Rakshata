Configuration script
=====================

This script enable the creation of a zip archive containing the pages of the volume/chapter being feed to it.
The script can either handle a single archive in the current active directory, or several of them by considering any directory in the active directory as containing a chapter, and will create archives whose name will be guessed as 'Chapitre_$nameOfDirectory'
Basically, name your directories as the # of the chapter and you're good to go.

Be carefull, the script will rename pages according to its internal logic (pageNumber.ext) and will add a config.dat
===

I plan to rewrite it in a better looking UI but don't really have the time lately :X
