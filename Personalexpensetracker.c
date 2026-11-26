#include <stdio.h>
int main() {
    char cat[5][15] = {"Food", "Travel", "Books", "Entertainment", "Others"};
    float alloc[5] = {0}, spent[5] = {0}, amt[100];
    int type[100], n = 0;
    float budget, left, total = 0, a, x, shortfall, avail, take, daily, freeAmt;
    int ch, c, s, ok, max;

    printf("=== PERSONAL EXPENSE TRACKER (Ledger Logic) ===\n");

    printf("Enter monthly budget (Rs.): ");
    ok = scanf("%f", &budget);
    while (ok != 1 || budget < 1) {
        while (getchar() != '\n');
        printf("Invalid! Enter a valid budget: ");
        ok = scanf("%f", &budget);
    }

    daily = budget / 30;
    printf("\n====== YOUR BUDGET PLAN ======\n");
    printf("Daily limit  : Rs. %.2f\n", daily);
    printf("Weekly limit : Rs. %.2f\n", daily * 7);
    printf("\n:) If you skip spending for just 5 days, you save Rs. %.2f\n", daily * 5);
    printf("   That can fund a nice trip with friends or a shopping treat!\n");

    left = budget;
    printf("\nSet your priorities: how much do you want to spend on each category?\n");
    for (int i = 0; i < 5; i++) {
        if (left <= 0) {
            continue;
        }
        printf("%-15s (max Rs. %.2f): ", cat[i], left);
        ok = scanf("%f", &x);
        while (ok != 1 || x < 0 || x > left) {
            while (getchar() != '\n');
            printf("Invalid! Enter between 0 and %.2f: ", left);
            ok = scanf("%f", &x);
        }
        alloc[i] = x;
        left -= x;
    }
    freeAmt = left;
    printf("Unallocated savings: Rs. %.2f\n", freeAmt);

    do {
        printf("\n1. Add expense\n2. View expenses\n3. Summary\n4. Exit\nChoice: ");
        ok = scanf("%d", &ch);
        while (ok != 1 || ch < 1 || ch > 4) {
            while (getchar() != '\n');
            printf("Invalid! Enter 1 to 4: ");
            ok = scanf("%d", &ch);
        }

        if (ch == 1 && n < 100) {
            for (int i = 0; i < 5; i++) {
                printf("  %d. %s\n", i + 1, cat[i]);
            }
            printf("Category (1-5): ");
            ok = scanf("%d", &c);
            while (ok != 1 || c < 1 || c > 5) {
                while (getchar() != '\n');
                printf("Invalid! Enter 1 to 5: ");
                ok = scanf("%d", &c);
            }
            c--;

            printf("Amount (Rs.): ");
            ok = scanf("%f", &a);
            while (ok != 1 || a <= 0) {
                while (getchar() != '\n');
                printf("Invalid! Enter an amount greater than 0: ");
                ok = scanf("%f", &a);
            }

            shortfall = a - (alloc[c] - spent[c]);
            spent[c] += a;
            total += a;
            type[n] = c;
            amt[n] = a;
            n++;

            if (shortfall > 0.001) {
                printf("\n%s budget is finished! Short by Rs. %.2f. Where will you adjust from?\n", cat[c], shortfall);
                while (shortfall > 0.001) {
                    for (int i = 0; i < 5; i++) {
                        if (i != c && alloc[i] - spent[i] > 0.001) {
                            printf("  %d. %s (Rs. %.2f available)\n", i + 1, cat[i], alloc[i] - spent[i]);
                        }
                    }
                    if (freeAmt > 0.001) {
                        printf("  6. Unallocated savings (Rs. %.2f)\n", freeAmt);
                    }
                    printf("  0. Don't adjust (overspend)\nChoice: ");
                    ok = scanf("%d", &s);
                    while (ok != 1 || s < 0 || s > 6) {
                        while (getchar() != '\n');
                        printf("Invalid! Enter 0 to 6: ");
                        ok = scanf("%d", &s);
                    }
                    if (s == 0) {
                        break;
                    }

                    if (s == 6) {
                        avail = freeAmt;
                    } else if (s - 1 == c) {
                        avail = 0;
                    } else {
                        avail = alloc[s - 1] - spent[s - 1];
                    }
                    if (avail <= 0.001) {
                        printf("  Nothing available there!\n");
                        continue;
                    }

                    take = avail;
                    if (shortfall < avail) {
                        take = shortfall;
                    }
                    if (s == 6) {
                        freeAmt -= take;
                    } else {
                        alloc[s - 1] -= take;
                    }
                    alloc[c] += take;
                    shortfall -= take;
                    printf("  Moved Rs. %.2f to %s.\n", take, cat[c]);
                }
                if (shortfall > 0.001) {
                    alloc[c] += shortfall;
                }
            }

            printf("\n%s: planned Rs. %.2f, spent Rs. %.2f, left Rs. %.2f\n", cat[c], alloc[c], spent[c], alloc[c] - spent[c]);
            printf("Available this month: Rs. %.2f\n", budget - total);
            if (total > budget) {
                printf("!!! ALERT: BUDGET EXCEEDED by Rs. %.2f !!!\n", total - budget);
            } else if (total >= 0.8 * budget) {
                printf("! WARNING: %.0f%% of your budget is used.\n", total * 100 / budget);
            }
        } else if (ch == 2) {
            if (n == 0) {
                printf("No expenses yet.\n");
            }
            for (int i = 0; i < n; i++) {
                printf("%d. %-15s Rs. %.2f\n", i + 1, cat[type[i]], amt[i]);
            }
        } else if (ch == 3) {
            max = 0;
            printf("\n====== SUMMARY ======  Budget: Rs. %.2f\n", budget);
            printf("%-15s %10s %10s %10s\n", "Category", "Planned", "Spent", "Left");
            for (int i = 0; i < 5; i++) {
                printf("%-15s %10.2f %10.2f %10.2f\n", cat[i], alloc[i], spent[i], alloc[i] - spent[i]);
                if (spent[i] > spent[max]) {
                    max = i;
                }
            }
            printf("Total spent: Rs. %.2f\n", total);
            if (n > 0) {
                printf("Highest spending: %s (Rs. %.2f)\n", cat[max], spent[max]);
            }
            printf("Available this month: Rs. %.2f\n", budget - total);
            if (total > budget) {
                printf("!!! ALERT: BUDGET EXCEEDED by Rs. %.2f !!!\n", total - budget);
            } else {
                if (total >= 0.8 * budget) {
                    printf("! WARNING: %.0f%% of your budget is used.\n", total * 100 / budget);
                }
                printf(":) Great job! Save Rs. %.2f and plan that trip or shopping!\n", budget - total);
            }
        }
    } while (ch != 4);

    printf("Goodbye!\n");
    return 0;
}
