#include <stdio.h>
#include <string.h>

int main() {
    // Open the original model
    FILE *in = fopen("assets/models/sword.obj", "r");
    // Create a new file for writing
    FILE *out = fopen("assets/models/sword_fixed.obj", "w");
    
    if (!in || !out) {
        printf("Error opening files! (Check the file path)\n");
        return 1;
    }

    char line[512];
    int converted_quads = 0;

    while (fgets(line, sizeof(line), in)) {
        // If the line starts with 'f ' (face)
        if (line[0] == 'f' && line[1] == ' ') {
            char v1[64], v2[64], v3[64], v4[64], extra[64];
            
            // Try to read 4 or more vertex blocks from the line
            int count = sscanf(line, "f %s %s %s %s %s", v1, v2, v3, v4, extra);
            
            if (count == 4) {
                // If it contains exactly 4 vertices (QUAD), split it into 2 TRIANGLES!
                fprintf(out, "f %s %s %s\n", v1, v2, v3);
                fprintf(out, "f %s %s %s\n", v1, v3, v4);
                converted_quads++;
            } else {
                // If it has 3 vertices (already a triangle), write it without changes
                fprintf(out, "%s", line);
            }
        } else {
            // Copy all other lines (v, vt, vn, etc.) as they are
            fprintf(out, "%s", line);
        }
    }

    fclose(in);
    fclose(out);
    printf("Done! %d quads have been converted to triangles.\n", converted_quads);
    return 0;
}