                     ____________________________________
                    |                                    |
                    |                                    |
                    |                                    |                      
                    |           START PROGRAM            |
                    |____________________________________|
                                     |
                                     |
                                     |
                                     |                             
                                     |                               
┌───────────────────────────────────────────────────────────────────────────────┐
│                                                                               │
│                           DISPLAY MAIN MENU                                   │
│                                                                               │
│ 1. Add new city(ies)      4. Edit city          7. Display roads              │
│ 2. Add roads              5. Search city        8. Display all data           │
│ 3. Add road budget        6. Display cities     9. Exit                       │
│                                                                               │
└───────┬─────────┬──────────┬────────┬───────────┬───────────┬────────┬────────┘
        │         │          │        │           │           │        │
        ▼         ▼          ▼        ▼           ▼           ▼        ▼
┌─────────────┐ ┌───────┐ ┌───────┐ ┌───────┐ ┌─────────┐ ┌───────┐ ┌───────┐
│ Add Cities  │ │ Roads │ │Budget │ │ Edit  │ │ Search  │ │Cities │ │ Roads │
│ - Get count │ │ - Show│ │ - Show│ │ - Get │ │ - Get   │ │ - List│ │ - Show│
│ - Get names │ │ cities│ │ roads │ │ index │ │ index   │ │ all   │ │ matrix│
│ - Assign IDs│ │ - Get │ │ - Get │ │ - Get │ │ - Display│ │ cities│ │       │
│ - Save file │ │ pairs │ │ budget│ │ new   │ │ details │ │       │ │       │
└───────┬─────┘ │ - Upd │ │ - Upd │ │ name  │ └─────────┘ └───────┘ └───┬───┘
        │       │ matrix│ │ matrix│ │ - Save│                             │
        │       └───┬───┘ └───┬───┘ └───────┘                             │
        │           │         │                                           │
        │           │         │                                           │
        ▼           ▼         ▼                                           ▼
┌───────────────────────────────────────────────────────────────────────────────┐
│                                                                               │
│                           DISPLAY ALL DATA (Option 8)                          │
│                                                                               │
│ - Show cities list                                                            │
│ - Show roads adjacency matrix                                                 │
│ - Show budgets adjacency matrix                                               │
│                                                                               │
└───────────────────────────────┬───────────────────────────────────────────────┘
                                │
                                ▼
┌───────────────────────────────────────────────────────────────────────────────┐
│                                                                               │
│                               EXIT (Option 9)                                 │
│                                                                               │
│ - Save all data to files                                                      │
│ - Terminate program                                                           │
│                                                                               │
└───────────────────────────────────────────────────────────────────────────────┘