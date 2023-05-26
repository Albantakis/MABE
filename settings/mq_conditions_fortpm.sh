
# Lines starting with # are ignored
# Lists must not contain white space (i.e. "1,2,3" OK, "1, 2, 3" Not OK)

# REPS = [FIRST REP] [LAST REP]
# if experiments are up to 3-digits, then start at 101. If up to 4-digits start at 1001, etc.
# This convention solves sorting and zero-padding problems later.
REPS = 101 102

# Settings to override the config files, but will not be varied
# Multiple CONSTANT definitions will be added together.

# NoA
#CONSTANT = GLOBAL-updates 50000 BRAIN_MARKOV_ADVANCED-recordIOMap 0 WORLD_PATHFOLLOW-useRandomTurnSymbols 0 WORLD_PATHFOLLOW-evaluationsPerGeneration 1
#CONSTANT = GLOBAL-updates 1 GLOBAL-initPop population_loader.plf WORLD-worldType TPM_GENERATOR BRAIN_MARKOV_ADVANCED-recordIOMap 0 WORLD_PATHFOLLOW-useRandomTurnSymbols 0 WORLD_PATHFOLLOW-evaluationsPerGeneration 1
#CONSTANT = GLOBAL-updates 1 GLOBAL-initPop population_loader.plf GLOBAL-mode analyze BRAIN_MARKOV_ADVANCED-recordIOMap 0 WORLD_PATHFOLLOW-useRandomTurnSymbols 0 WORLD_PATHFOLLOW-evaluationsPerGeneration 1

# A
#CONSTANT = GLOBAL-updates 50000 BRAIN_MARKOV_ADVANCED-recordIOMap 0 WORLD_PATHFOLLOW-useRandomTurnSymbols 0 WORLD_PATHFOLLOW-evaluationsPerGeneration 1
#CONSTANT = GLOBAL-updates 1 GLOBAL-initPop population_loader.plf WORLD-worldType TPM_GENERATOR BRAIN_MARKOV_ADVANCED-recordIOMap 0 WORLD_PATHFOLLOW-useRandomTurnSymbols 1 WORLD_PATHFOLLOW-evaluationsPerGeneration 1 WORLD_PATHFOLLOW-turnSymbolsCount 4
#CONSTANT = GLOBAL-updates 1 GLOBAL-initPop population_loader.plf GLOBAL-mode analyze BRAIN_MARKOV_ADVANCED-recordIOMap 0 WORLD_PATHFOLLOW-useRandomTurnSymbols 1 WORLD_PATHFOLLOW-evaluationsPerGeneration 10 WORLD_PATHFOLLOW-turnSymbolsCount 4
#CONSTANT = GLOBAL-updates 1 GLOBAL-initPop population_loader.plf GLOBAL-mode visualize BRAIN_MARKOV_ADVANCED-recordIOMap 1 WORLD_PATHFOLLOW-useRandomTurnSymbols 1 WORLD_PATHFOLLOW-evaluationsPerGeneration 10 WORLD_PATHFOLLOW-turnSymbolsCount 4

# B
#CONSTANT = GLOBAL-updates 150 BRAIN_MARKOV_ADVANCED-recordIOMap 0 WORLD_PATHFOLLOW-randomTurnSymbols -1 WORLD_PATHFOLLOW-turnSymbolsCount 2
CONSTANT = GLOBAL-updates 1 GLOBAL-initPop population_loader.plf WORLD-worldType TPM_GENERATOR BRAIN_MARKOV_ADVANCED-recordIOMap 0 WORLD_PATHFOLLOW-randomTurnSymbols -1 WORLD_PATHFOLLOW-evaluationsPerGeneration 1 WORLD_PATHFOLLOW-turnSymbolsCount 2
#CONSTANT = GLOBAL-updates 1 GLOBAL-initPop population_loader.plf GLOBAL-mode analyze BRAIN_MARKOV_ADVANCED-recordIOMap 0 WORLD_PATHFOLLOW-randomTurnSymbols -1 WORLD_PATHFOLLOW-evaluationsPerGeneration 10 WORLD_PATHFOLLOW-turnSymbolsCount 2
#CONSTANT = GLOBAL-updates 1 GLOBAL-initPop population_loader.plf GLOBAL-mode visualize BRAIN_MARKOV_ADVANCED-recordIOMap 1 WORLD_PATHFOLLOW-randomTurnSymbols -1 WORLD_PATHFOLLOW-evaluationsPerGeneration 10 WORLD_PATHFOLLOW-turnSymbolsCount 2



# VAR = [SHORT NAME]	[REAL NAME]	[conditon1,condition2,etc.]
# Short name is used in this file, and also determines output directory names
#VAR = RT	WORLD_PATHFOLLOW-useRandomTurnSymbols   
#VAR = WOA	WORLD_PATHFOLLOW-useRandomTurnSymbols 1,
#VAR = MM	WORLD_PATHFOLLOW-mapNames
#VAR = FLP	WORLD_PATHFOLLOW-addFlippedMaps
#VAR = WAMM	BRAIN_MARKOV_ADVANCED-recordIOMap 1


# Alternatively to VAR/EXCEPT, conditions can achieve a similar effect and still respect EXCEPT declarations
#CONDITIONS = TSK=1.0,3.0  RF1="some message here"  RF2=2,[1,2,3,4]
#CONDITIONS = RT=0,1

# EXCEPT = [SHORT NAME]=[condition],[SHORT NAME]=[condition],etc.
# If all name/condition pairs are met for any EXCEPT, then that combination will not be run.
#EXCEPT = WOA=20, 

# list of setting files (.cfg) which you want MABE to load with the -f option. files will be loaded in the order provided.
SETTINGS = settings.cfg,settings_organism.cfg,settings_world.cfg

# list of files used by MABE (i.e. maps, images, etc.) and you can use rep replacement {{rep}} syntax
# these files will be copied to the working directory if necessary
#OTHERFILES = my_file.txt

# a population_loader.plf file can be created using the contents here
# be sure to set the -p GLOBAL-initPop population_loader.plf in CONSTANT definitions above
#PLF = MASTER = 'A4_C0__/{{rep}}/LOD_organisms.csv'
# or the following 2 lines with rep replacement works:
#PLF = MASTER = greatest 1 by ID from { 'A4_C0__/{{rep}}/LOD_organisms.csv' }
#PLF = MASTER = collapse some_var
-----

# JOBNAME will be appended before condition (C*) identifier - if "NONE" then job will have not JOBNAME
JOBNAME = B2_test

# EXECUTABLE is the name of program to run, may include a path
EXECUTABLE = ./mabe
#EXECUTABLE = mabe.exe

-----
# comma separate the mail type list ex: FAIL,END,REQUEUE

# HPCC (not used for indefinite mode)
#HPCC_PARAMETERS = #SBATCH --constraint="[intel14|intel16|intel18]"
#HPCC_PARAMETERS = #SBATCH --ntasks=1
#HPCC_PARAMETERS = #SBATCH --cpus-per-task=1
#HPCC_PARAMETERS = #SBATCH --time=03:55:00  #WARNING indefinite requires 03:51:00 or greater
#HPCC_PARAMETERS = #SBATCH --mem=2G
#HPCC_PARAMETERS = #SBATCH --mail-type=FAIL
