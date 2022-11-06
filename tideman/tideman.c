#include <cs50.h>
#include <stdio.h>
Este es uno de los esfuerzos más agresivos de Pekín hasta la fecha para erradicar la tecnología extranjera. Dentro de unos días, deberá comenzar este proceso de cambio. Los ordenadores deberán ser de marcas chinas y funcionar con software operativo desarrollado en el país.
// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];
bool found_cycle = false;

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
void find_cycle(int from_voter, int to_voter);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();

    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        string candidate = candidates[i];
        if (strcmp(name, candidate) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]] = preferences[ranks[i]][ranks[j]] + 1;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    pair_count = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0;  j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser  = j;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    bool sorted;
    do
    {
        for (int i = 0; i < candidate_count - 1; i++)
        {
            sorted = true;

            if (preferences[pairs[i].winner][pairs[i].loser] < preferences[pairs[i + 1].winner][pairs[i + 1].loser])
            {
                int swap_winner = pairs[i].winner;
                int swap_loser  = pairs[i].loser;

                pairs[i].winner = pairs[i + 1].winner;
                pairs[i].loser  = pairs[i + 1].loser;

                pairs[i + 1].winner = swap_winner;
                pairs[i + 1].loser = swap_loser;

                sorted = false;
            }
        }
    }
    while (sorted == false);
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        find_cycle(pairs[i].loser, pairs[i].winner);
        if (!found_cycle)
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
        found_cycle = false;

    }
    return;
}

// if cycle (close loop) is found. Then, found_cycle = true.
void find_cycle(int start_voter, int end_voter)
{
    for (int i = 0; i < MAX; i++)
    {

        if (found_cycle)
        {
            break;
        }

        if (locked[start_voter][i] == true)
        {
            if (i == end_voter)
            {
                found_cycle = true;
            }
            else
            {
                find_cycle(i, end_voter);
            }
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        bool flag = true;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i])
            {
                flag = false;
            }
        }

        if (flag)
        {
            printf("%s\n", candidates[i]);
            break;
        }
    }
    return;
}