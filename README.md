# Algorithms Project
 In this team project, we applied the LSH and Hypercube algorithms , using various metrics ( euclidean , continuous/discrete Fréchet) in order to solve the k-nearest neighbors problem. Subsequently we generalized the procedure to create k clusters, with k-means initialization


ΠΟΥΡΝΑΡΑΣ ΓΕΩΡΓΙΟΣ sdi1800162 ΑΡΒΑΝΙΤΕΛΛΗΣ ΒΑΙΟΣ ΙΑΣΩΝ sdi1800010

Οδηγιες χρήσης

make

./cluster -i input_small_id -c exampleconfig.txt -o clusterout -complete -m LSH
./lsh -i input_small_id -q query_small_id -k 4 -L 5 -o results -N 1 -R 10000
./cube -i input_small_id -q query_small_id -k 14 -M 10 -probes 2 -o out -N 1 -R 10000   

Στο cluster to - complete ειναι optinal 


ΠΟΥΡΝΑΡΑΣ ΓΕΩΡΓΙΟΣ sdi1800162

vHash 

Σε αυτα τα αρχεια γινονται οι διαδικασιες για τον lsh
Εχουμε σαν κυρια δομη την vector<vector<vector<pair<std::string,vector<double>*>>*>*>*,
η οποια στην ουσια ειναι 
Πινακας απο L πινακες κατακερματισμου με πληθος (tableSize = Total vectors/ 16) buckets
το οποιο καθε bucket εχει μια λιστα απο pair< itemId , vector of coordinates>

Επισης εχουμε ενα πινακα που εχει τις hashFunctions του καθε hash 

H insert βαζει καθε διανυσμα σε ολα τα hash , h search βρισκει τους nearest neighboors
και η rangeSearch ψαχνει τους γειτονες του query μεσα σε μια ακτινα

hashFunctions

Σε αυτα τα αρχεια γινεται η υλοποιηση των συναρτησεων κατακερματισμου
Αποφασισαμε οι g και h  να επιστρεφουν θετικες τιμες ετσι ωστε να μην
δημιουργουνται προβληματα οταν γινεται indexing στα hash

fileHandler

Σε αυτα τα αρχεια εχουμε γενικες συναρτησεις πχ για το διαβασμα απο το αρχειο
αλλα και τις bruteforce συναρτησεις οι οποιες δεν ανηκουν ουτε στο lsh ουτε στο hypercube

Cluster_main

Στο range search αν δεν επιστραφουν αρκετοι γειτωνες κανουμε bruteforce
Οι γειτονες επιστεφονται σε λιστες και τα clusters ειναι maps 


ΑΡΒΑΝΙΤΕΛΛΗΣ ΒΑΙΟΣ ΙΑΣΩΝ sdi1800010

Για την υλοποίηση της random hypercube projection δημιουργήθηκε η δομή Cube όπου περιέχει ένα array από τυχαία strings(0/1) σε μορφή bitset.
To hash table είναι ενα vector με std::pairs των id των αντικειμένων και των αντίστοιχων διανυσμάτων,όπως και η αρχική δομή vectorspace που περιέχει όλα τα entries.
Κάθε τιμή που επιστρέφεται από τις hash function στην εισαγωγή στοιχείων αντιστοιχίζεται(αν δεν έχει γίνει ήδη) με μια random τιμή (1/0) και το ζευγάρι αυτό σώζεται σε ένα std map για μελλοντική χρήση.

Για κάθε query ακολουθείται η ίδια διαδικασία με τα δεδομένα,χωρίς βέβαια να γίνει εκχώρηση στο hash table, και συγκεντρώνονται τα πιθανά του neighbors στο results vector,το οποίο στη συνέχεια  κάνουμε ξεκαθάρισμα και κρατάμε τους N κοντινότερους γείτονες
Στη συνέχεια συλλέγονται και οι ανονικοί nearest neighbors αλλά και οι nearest in range neighbors με brute force από τις συναρτήσεις getTrueNear και getInRadius αντίστοιχα.Αν δεν βρεθεί το αντίστοιχο bucket για το query,εκτυπώνεται αντίστοιχο μήνυμα.

Με παρόμοιο τρόπο δουλεύει και η συνάρτηση cuberange στο αρχείο cubefun.cpp για την εκτέλεση του clustering,παίρνωντας ως όρισμα τις κατάλληλες δομές από το main πρόγραμμα και γυρνώντας ένα ένα vector με όσους υποψήφιους neighbors έχουν απόσταση<Range.
Η αρχικοποίηση των δομών αυτών γίνεται στο cluster_main αρχείο και οι παράμετροί τους δίνονται από το config file που περνάει από το command line .

Στο lloyds algorith clustering διαλέγονται στην αρχή centroids και μοιράζονται τα στοιχεία στα πρώτα clusters,και μετά σε κάθε βήμα διαλέγονται νέα centroids που παράγονται απο τον μέσο όρο του αθροίσματος των στοιχείων του έκαστου cluster και μοιράζονται ξανά με brute force όλα τα στοιχεία στα κοντινότερα τους cluster,μέχρι να γίνουν λιγότερες από
THRESHOLD(#define THRESHOLD) μετακινήσεις απο cluster σε cluster.Tα sillhouetes υπολογίζονται με brute force μετρήσεις από κάθε στοιχείο σε κάθε άλλο του δικού του και του κοντινότερου άλλου cluster.




proj2main:
Η αρχικοποίηση των δομών της κάθε μεθόδου γίνεται ανάλογα τα ορίσματα του προγράμματος.Για την διαχείρηση του κύβου καλείται στο κάθε query η cubeNnearest
όπου γυρίζει τον nearest neighbor του κάθε query(approximation σύμφωνα με τον κύβο).

proj2cluster:
Αν δοθεί λάθος συνδυασμός metric,assignment, γίνεται exit.Tα αρχικά kmeans κεντροειδή δίνονται απο την getKmeansCentroids.Τα assignments αλλά και οι αρχικοποιήσεις τους
γίνονται ανάλογα με τις αρχικές επιλογές στο command line.Ο χρόνος που χρειάζεται το clustering χρονολογείται για τα τελικά αποτελέσματα και η update των 
κεντροειδών γίνεται όπως την 1η εργασία για vector update,αλλά μέσω της Mean_Frechet_Update για curve update.

Mean Frechet Update:Για τον υπολογισμό του τελικού κεντροειδούς αρχικά δημιουργείται ένα αντικείμενο bintree(array που προσωμειώνει την λειτουργία ενός δέντρου) και καλείται η μέθοδος mean σε αυτό.

Bintree:Στο bintree αποθηκεύεται το κάθε cluster,όπου τα μέλη του στη συνέχεια μοιράζονται σε τυχαίες θέσεις ενός πίνακα,και επαναληπτικά,ανα δύο στοιχεία υπολογίζεται το optimal traversal τους(όπως στις διαφάνειες)
και δημιουργείται ένας νέος πίνακας με n/2 στοιχεία,μέχρι να μείνει 1 μονο στοιχείο,το τελικό mean του cluster.Σε κάθε optimal traversal γίνεται filtering(max στοιχεία=στοιχεία αρχικού διανύσματος) ώστε να βελτιωθεί ο χρόνος εκτέλεσης του αλγορίθμου.

Testing.cpp:

./test traversal exampleconfig.txt
 ./test discrete exampleconfig.txt
 ./test filter exampleconfig.txt
./test pair exampleconfig.txt

Με τις εξής εντολές ελέγχονται οι αντίστοιχες συναρτήσεις.
Στο Traversal δίνονται δυο αρχικά "dummy" διανύσματα και απλά εκτυπώνεται το αντίστοιχο optimal traversal τους.

Στo filter δίνεται αρχικό διάνυσμα,οι παράμετροι του filtering και στη συνέχεια το φιλτραρισμένο αποτέλεσμα.

Στο pair λειτουργούμε με όμοιο τρόπο με το filter.

Στο discrete με δυο dummy διανυσματα εκτυπωνουμε καποιες αποστασεις

Για δοκιμες τρεξιματως εχουμε το αρχειο testakia.txt που εχει ολους τους δυνατους συνδιασμους 

Καθε που κανουμε insert ή αναζητηση σε lsh frechet τοτε στο querry κανουμε snap h filter 
