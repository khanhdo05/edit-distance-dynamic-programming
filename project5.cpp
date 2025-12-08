/*
 * CSC-301
 * project5.cpp
 * Fall 2025
 *
 * Partner 1: Khanh Do
 * Partner 2: Gun Woo Kim
 * Date: Dec 5, 2025
 */

#include "project5.hpp"

using namespace std;

/*
 * ED - solves the given ED problem using DP or Memoization.
 *
 * INPUTS:
 * string &src: the source string to edit on.
 * string &dest: the destination string to make edits to.
 * bool useASM: flag to indicate if ASM should be used.
 * bool useDP: flag to indicate if DP (true) or Memoization (false) should be used.
 * 
 * OUTPUTS:
 * vector<int>: the filled table of edits computed.
 */
vector<int> ED(string &src, string &dest, bool useASM, bool useDP) {
    vector<int> table;
    if (useDP) {
        table = ED_DP(src, dest, useASM);
    } else {
        table = ED_Mem(src, dest, useASM);
    }
    return table;
}

/*
 * ED_DP - create and fill the table iteratively using dynamic programming.
 *
 * INPUTS:
 * string &src: the source string to edit on.
 * string &dest: the destination string to make edits to.
 * bool useASM: flag to indicate if ASM should be used.
 * 
 * OUTPUTS:
 * vector<int>: the filled table of edits computed.
 */
vector<int> ED_DP(string &src, string &dest, bool useASM) {
    // Get string lengths for ease of use.
    int n = src.length(); // Number of rows (-1).
    int m = dest.length(); // Number of cols (-1).
    int numCols = m+1;

    // Setup the table, size (n+1)x(m+1), filled with -1s.
    vector<int> table( (n+1)*(m+1), -1 );

    // Fill in the base cases.
    for (int i = 0; i <= n; i++) {
        // Delete from src to get to empty dest.
        table[i*numCols + 0] = i;
    }

    for (int j = 0; j <= m; j++) {
        // Insert into empty src to get to dest.
        table[0*numCols + j] = (useASM ? 0 : j);
    }

    // Iteratively fill in the rest of the table.
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            // Compute the minimum edits for table[i][j].
            if (src[i - 1] == dest[j - 1]) {
                // Characters match, no edit needed.
                table[i*numCols + j] = table[(i - 1)*numCols + (j - 1)];
            } else {
                // Characters don't match, consider all edits.
                int ins = table[i*numCols + (j - 1)] + 1;
                int del = table[(i - 1)*numCols + j] + 1;
                int sub = table[(i - 1)*numCols + (j - 1)] + 1;

                // Save the min of three options.
                table[i*numCols + j] = min({ins, del, sub});
            }
        }
    }

    // Return the completed table.
    return table;
}

/*
 * ED_Mem - create and fill the table recursively using memoization.
 *
 * INPUTS:
 * string &src: the source string to edit on.
 * string &dest: the destination string to make edits to.
 * bool useASM: flag to indicate if ASM should be used.
 * 
 * OUTPUTS:
 * vector<int>: the filled table of edits computed.
 */
vector<int> ED_Mem(string &src, string &dest, bool useASM) {
    // Get string lengths for ease of use.
    int n = src.length(); // Number of rows (-1).
    int m = dest.length(); // Number of cols (-1).
    int numCols = m+1;

    // Setup the table, size (n+1)x(m+1), filled with -1s.
    vector<int> table( (n+1)*(m+1), -1 );

    // Call the recursive helper to fill in the table.
    ED_Mem_helper(src, dest, useASM, numCols, n, m, table);

    // Return the completed table.
    return table;
}

/*
 * ED_Mem_helper - recursive helper function for memoization.
 *                 Note: numCols input for ease of indexing.
 * 
 * INPUTS:
 * string &src: the source string to edit on.
 * string &dest: the destination string to make edits to.
 * bool useASM: flag to indicate if ASM should be used.
 * int numCols: the number of columns in the table.
 * int i: the current row index.
 * int j: the current column index.
 * vector<int> &table: the table being filled.
 * 
 * OUTPUTS:
 * int: the minimum number of edits for src[0..i-1] to dest[0..j-1].
 */
int ED_Mem_helper(string &src, string &dest, bool useASM, int numCols,
                  int i, int j, vector<int> &table) {
    if (table[i*numCols + j] != -1) {
        // Already filled, return the memoized value.
        return table[i*numCols + j];
    }
    
    // Not filled, compute the value.
    if (i == 0) {
        // At the top row, need j insertions.
        table[i*numCols + j] = (useASM ? 0 : j);
    } else if (j == 0) {
        // At the left col, need i deletions.
        table[i*numCols + j] = i;
    } else if (src[i - 1] == dest[j - 1]) {
        // Characters match, no edit needed.
        table[i*numCols + j] = ED_Mem_helper(src, dest, useASM, numCols, i - 1, j - 1, table);
    } else {
        // Characters don't match, consider all edits.
        int cost = (src[i - 1] == dest[j - 1]) ? 0 : 1;

        int ins = ED_Mem_helper(src, dest, useASM, numCols, i, j - 1, table) + 1;
        int del = ED_Mem_helper(src, dest, useASM, numCols, i - 1, j, table) + 1;
        int sub = ED_Mem_helper(src, dest, useASM, numCols, i - 1, j - 1, table) + cost;
    
        // Save the min of three options.
        table[i*numCols + j] = min({ins, del, sub});
    }
    
    return table[i*numCols + j];
}

/*
 * getNumEdits - given a filled table, return the min number of edits.
 *
 * INPUTS:
 * string &src: the source string for ED.
 * string &dest: the destination string for ED.
 * vector<int> &table: the filled table from ED.    
 * 
 * OUTPUTS:
 * int: the minimum number of edits required to transform src into dest.
 */
int getNumEdits(string &src, string &dest, vector<int> &table) {
    int numEdits = table[src.length() * (dest.length() + 1) + dest.length()];
    return numEdits;
}

/*
 * getEdits - reconstruct the optimal edits given a filled table.
 *
 * INPUTS:
 * string &src: the source string for ED.
 * string &dest: the destination string for ED.
 * vector<int> &table: the filled table from ED.
 *
 * OUTPUTS:
 * vector<Edit>: a vector containing the edits required to transform src into dest.
 */
vector<Edit> getEdits(string &src, string &dest, vector<int> &table) {
    vector<Edit> edits;
    int n = src.length();
    int m = dest.length();
    int numCols = m + 1;

    // Init i, j to bottom-right of the table.
    int i = n, j = m;

    // Backtrack to find the edits.
    while (i > 0 || j > 0) {
        if (i == 0) {
            // At the top row, insert the remaining chars from dest
            edits.push_back(Edit("ins", '\0', dest[j-1], 0));
            j--;
            continue;
        }

        if (j == 0) {
            // At the left col, delete the remaining chars from src
            edits.push_back(Edit("del", src[i-1], '\0', i-1));
            i--;
            continue;
        }

        if (i > 0 && j > 0 && src[i-1] == dest[j-1]) {
            // match
            edits.push_back(Edit("match", src[i-1], dest[j-1], i-1));
            i--; 
            j--;
        } else {
            int curr = table[i*numCols + j];

            // insert
            if (j > 0 && table[i*numCols + (j-1)] + 1 == curr) {
                edits.push_back(Edit("ins", '\0', dest[j-1], i));
                j--;
            }
            // delete
            else if (i > 0 && table[(i-1)*numCols + j] + 1 == curr) {
                edits.push_back(Edit("del", src[i-1], '\0', i-1));
                i--;
            }
            // substitution
            else {
                edits.push_back(Edit("sub", src[i-1], dest[j-1], i-1));
                i--; 
                j--;
            }
        }
    }

    return edits;
}

/*
 * printTable: prints the table so it looks nice.
 *
 * INPUTS
 * string src: the source string for ED (or ASM).
 * string dest: the destination string for ED (or ASM).
 * vector<int> &table: the table itself.
 *
 * OUTPUTS
 * string tableString.str(): the pretty string.
 */
string printTable(string &src, string &dest, vector<int> &table) {
    // Create the stringstream for the table.
    stringstream tableString;

    // Shift the first row for the dest letters and write the letters.
    // tableString << setw(labelWidth) << fixed;
    tableString << " " << " " << " " << " ";
    for (int j=0; j<dest.length(); j++) {
        // tableString << setprecision(prec) << setw(numWidth) << fixed;
        tableString << dest[j] << " ";
    }
    tableString << endl;

    // Write each line of the table, with the proper src letter at front.
    int entry;
    for (int i=0; i<src.length()+1; i++) {
        // tableString << setw(labelWidth) << fixed;
        if (i > 0) {
            tableString << src[i-1] << " ";
        } else {
            tableString << " " << " ";
        }
        for (int j=0; j<dest.length()+1; j++) {
            // tableString << setprecision(prec) << setw(numWidth) << fixed;
            entry = table[i*(dest.length()+1) + j];
            if (entry > -1) {
                tableString << entry << " "; // filled
            } else {
                tableString << "X" << " "; // unfilled
            }
        }
        tableString << endl;
    }
    tableString << endl;

    return tableString.str();
}