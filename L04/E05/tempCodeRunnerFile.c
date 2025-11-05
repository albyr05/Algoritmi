for (i = start; i < N; i++){
        sol[pos] = v[i];
        bool compatibile = true;
        for (int j = 0; j < pos; j++) {
            if (check_compatibility(sol[j], v[i]) == false) {
                compatibile = false;
                break;
            }
        }
        if (compatibile) {       // if the activity is compatibile with all the previous i can proceed with recursion
            int d = v[i].end - v[i].start;      
            current_d += d;         // adding the duration of the just insert activity
            max_subests(pos+1, sol, max_d, current_d, N, v, best_sol, len, i + 1);      // proceed with recursion
            current_d -= d;         // backtracking --> removes the activity 
        }   
    
}