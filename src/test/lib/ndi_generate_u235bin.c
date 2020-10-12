#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main() {
	int i, j;
	int str_size = 512;
	char text[str_size], zaid[512];
	char *keyword_list[] = {"zaid", "date_source", "date_processed", "library_name",
				"info", "data_type", "num_grps", "up_grps", "pn_order",
				"num_reac", "last_n_grp", "num_sec_parts", "e_bounds",
				"at_wgt", "awr", "temp", "sig_0", "norm", "fiss_q",
				"wgts", "vel", "sig_tot", "kerma", "heating", "nu_tot",
				"nu_pr", "nu_del", "nu_sig_f_tot", "chi_mat_tot",
				"chi_mat_pr", "chi_mat_del", "pn_full", "pn_down_full",
				"sig_reac", "rprod_all", "rprod_rmo", "rprod_few",
				"sec_part_types", "sec_part_zaids" };
	char *string1[] = {"src/test/lib/u235asc2", "src/test/lib/u235bin"};
	char zero = '0';
	char help[20];
	float fnum;
        double dnum;
	FILE *file, *file2;
	int ierr, num, ng, ng_scatt, pn, nreac, nsec_parts, ngsec, pnsec, num_prod,
             mt, yes_continue, start, k;

	int num_keywords = sizeof(keyword_list) / sizeof(keyword_list[0]);
	long lnum;

	printf("Running: %s %s\n",string1[0], string1[1]);

	file =  fopen(string1[0], "rb");
	file2 = fopen(string1[1], "wb");
	strncpy(text, "", str_size);
	start = 0;
        while (fscanf(file, "%s", text) != EOF) {
		fwrite(text, 1, sizeof(text), file2);
		printf("Keyword: %s\n",text);
		if (start == 0) {
		    if (strcmp(text, "zaid") == 0)
			start = 1;
		}
		// END conditions
		if ( strcmp(text, "end") == 0 ) { // ||
		    strcpy(text, "end");
		    fwrite(text, 1, sizeof(text), file2);
		    break;
		}
		// zaid
		if ( strcmp(text, "zaid") == 0 ) {
                    fscanf(file, "%s", text);
		    // add "b" to end of zaid to denote binary
		    strcat(text,":b");
		    fwrite(text, 1, sizeof(text), file2);
		}
                // strings
                if ( strcmp(text, "date_source") == 0 ||
                     strcmp(text, "date_processed") == 0 ||
                     strcmp(text, "library_name") == 0 ||
		     strcmp(text, "info") == 0 ||
		     strcmp(text, "data_type") == 0
                ) {
		    fscanf(file, "%s", text);
		    yes_continue = 0;
		    while (yes_continue == 0) {
		        for (k = 0; k < num_keywords; ++k) {
			    if ( strcmp(text, keyword_list[k]) == 0 ) {
				printf("E1\n");
				yes_continue = 1;
			        break;
			    }
			}
			if (yes_continue == 0) {
                            fwrite(text, 1, sizeof(text), file2);
                            fscanf(file, "%s", text);
			}
		    }
		    if ( yes_continue == 1 ) {
			printf("E2\n");
                        fseek(file, -strlen(keyword_list[k]), SEEK_CUR);
		        strcpy(text, "break");
		        fwrite(text, 1, sizeof(text), file2);
		    }
		    continue; 
                }
		// integers
                if ( strcmp(text, "num_grps") == 0 ||
                     strcmp(text, "up_grps") == 0 ||
                     strcmp(text, "down_grps") == 0 ||
                     strcmp(text, "pn_order") == 0 ||
                     strcmp(text, "num_reac") == 0 ||
		     strcmp(text, "last_n_grp") == 0 ||
		     strcmp(text, "num_sec_parts") == 0
                ) {
                        fscanf(file, "%d", &num);
                        if ( strcmp(text, "num_grps") == 0 )
                                ng = num;
                        if ( strcmp(text, "pn_order") == 0 )
                                pn = num;
                        if ( strcmp(text, "num_reac") == 0 )
                                nreac = num;
			if ( strcmp(text, "num_sec_parts") == 0 ) {
				nsec_parts = num;
			}
			printf(" %d\n",num);
                        fwrite(&num, sizeof(num), 1, file2);
                }
		// doubles
                if ( strcmp(text, "at_wgt") == 0 ||
                     strcmp(text, "awr") == 0 ||
                     strcmp(text, "temp") == 0 ||
                     strcmp(text, "sig_0") == 0
                ) {
			fscanf(file, "%f", &fnum);
	                dnum = (double    ) fnum;
			fwrite(&dnum, sizeof(dnum), 1, file2);
                }
		// 4 doubles
                if ( strcmp(text, "norm") == 0 ) {
                        for (i = 0; i < 4; ++i) {
                                fscanf(file, "%f", &fnum);
                                dnum = (double    ) fnum;
                                fwrite(&dnum, sizeof(dnum), 1, file2);
                        }
                }
		// 6 doubles
		if ( strcmp(text, "fiss_q") == 0 ) {
			for (i = 0; i < 6; ++i) {
                                fscanf(file, "%f", &fnum);
                                dnum = (double    ) fnum;
                                fwrite(&dnum, sizeof(dnum), 1, file2);
                        }
		}
		// ng doubles
                if ( strcmp(text, "wgts") == 0 ||
                     strcmp(text, "vel") == 0 ||
                     strcmp(text, "sig_tot") == 0 ||
                     strcmp(text, "kerma") == 0 ||
		     strcmp(text, "heating") == 0 ||
		     strcmp(text, "kerma") == 0 ||
		     strcmp(text, "nu_tot") == 0 ||
		     strcmp(text, "nu_pr") == 0 ||
		     strcmp(text, "nu_del") == 0 ||
		     strcmp(text, "nu_sig_f_tot") == 0 ||
                     strcmp(text, "nu_sig_f_pr") == 0 ||
                     strcmp(text, "nu_sig_f_del") == 0 ||
                     strcmp(text, "chi_vec_tot") == 0 ||
                     strcmp(text, "chi_vec_pr") == 0 ||
                     strcmp(text, "chi_vec_del") == 0
                ) {
                        for (i = 0; i < ng; ++i) {
                                fscanf(file, "%f", &fnum);
        	                dnum = (double    ) fnum;
	                        fwrite(&dnum, sizeof(dnum), 1, file2);
                        }
                }
		// ng+1 doubles
                if ( strcmp(text, "e_bounds") == 0 ){
                        for (i = 0; i < ng+1; ++i) {
                                fscanf(file, "%f", &fnum);
        	                dnum = (double    ) fnum;
	                        fwrite(&dnum, sizeof(dnum), 1, file2);
                        }
                }
		// ng*ng doubles
		if ( strcmp(text, "chi_mat_tot") == 0 ||
		     strcmp(text, "chi_mat_pr") == 0 ||
 		     strcmp(text, "chi_mat_del") == 0 ) {
                        for (i = 0; i < ng*ng; ++i) {
                                fscanf(file, "%f", &fnum);
                                dnum = (double    ) fnum;
                                fwrite(&dnum, sizeof(dnum), 1, file2);
                        }
                }
		// pn_full/pn_down_full
		if ( strcmp(text, "pn_full") == 0 ||
		     strcmp(text, "pn_down_full") == 0 ) {
			ng_scatt = 1;
			if ( strcmp(text, "pn_full") == 0 ) ng_scatt = ng*ng;
			if ( strcmp(text, "pn_down_full") == 0 ) ng_scatt = ng*(ng+1)/2;
			for (i = 0; i < pn; ++i) {
				fscanf(file, "%d", &num);
				fwrite(&num, sizeof(num), 1, file2);
				for (j = 0; j < ng_scatt; ++j) {
					fscanf(file, "%f", &fnum);
					dnum = (double    ) fnum;
                                        fwrite(&dnum, sizeof(dnum), 1, file2);
				}
			}
		}
		// reaction cross sections
		if ( strcmp(text, "sig_reac") == 0 ) {
			for (j = 0; j < nreac; ++j) {
				fscanf(file, "%d %f", &num, &fnum);
				fwrite(&num, sizeof(num), 1, file2);
        	                dnum = (double   ) fnum;
	                        fwrite(&dnum, sizeof(dnum), 1, file2);
				for (i = 0; i < ng; ++i) {
                        	        fscanf(file, "%f", &fnum);
        	        	        dnum = (double    ) fnum;
	           	      		fwrite(&dnum, sizeof(dnum), 1, file2);
                        	}
			}
		}
		// reaction products
		if ( strcmp(text, "rprod_all") == 0 ||
		     strcmp(text, "rprod_rmo") == 0 ||
		     strcmp(text, "rprod_few") == 0 ) {
		    if ( nreac == 0 ) printf("ERROR in RPROD\n");
		    for (i = 0; i < nreac; ++i) {
			fscanf(file, "%d", &num); // mt number
			mt = num;
			fwrite(&num, sizeof(num), 1, file2);
                        fscanf(file, "%d", &num); // num products
			num_prod = num;
                        fwrite(&num, sizeof(num), 1, file2);
			if (num_prod == 0)
			    printf("Warning, no reaction products for MT %d\n", mt);
			for (j = 0; j < num_prod; ++j) {
			    fscanf(file, "%d", &num); // product
			    fwrite(&num, sizeof(num), 1, file2);
                            fscanf(file, "%d", &num); // number of product
                            fwrite(&num, sizeof(num), 1, file2);
			}
		    }
		}
		// long int (secondary particle)
                if ( strcmp(text, "sec_part_types") == 0 ) {
                        for (i = 0; i < nsec_parts; ++i) {
                                fscanf(file, "%ld", &lnum);
                                fwrite(&lnum, sizeof(long), 1, file2);
                        }
                }

		// strings (secondary particles)
		if ( strcmp(text, "sec_part_zaids") == 0 ) {
			for (i = 0; i < nsec_parts; ++i) {
				fscanf(file, "%s", text);
				fwrite(text, 1, sizeof(text), file2);
			}
		}
		// number of groups (secondary particles)
		if ( strncmp(text, "num_grps_", 9) == 0 ||
		     strncmp(text, "pn_order_", 9) == 0 ) {
			fscanf(file, "%d", &num);
			fwrite(&num, sizeof(num), 1, file2);
			if (strncmp(text, "num_grps_", 9) == 0)
			    ngsec = num;
			if (strncmp(text, "pn_order_", 9) == 0)
			    pnsec = num;
			
		}
		// ngsec doubles (secondary particles)
                if ( strncmp(text, "kerma_", 6) == 0 ||
		     strncmp(text, "heating_", 8) == 0 ) {
                        for (i = 0; i < ng; ++i) {
                                fscanf(file, "%f", &fnum);
                                dnum = (double    ) fnum;
                                fwrite(&dnum, sizeof(dnum), 1, file2);
                        }
                }
		// ngsec+1 doubles (secondary particles)
                if ( strncmp(text, "e_bounds_", 9) == 0 ) {
                        for (i = 0; i < ngsec+1; ++i) {
                                fscanf(file, "%f", &fnum);
                                dnum = (double    ) fnum;
                                fwrite(&dnum, sizeof(dnum), 1, file2);
                        }
                }
		// ngsec*ngsec doubles (secondary particles)
		if ( strncmp(text, "pn_prod_full_", 13) == 0 ) {
                        ng_scatt = 1;
                        for (i = 0; i < pnsec; ++i) {
                                fscanf(file, "%d", &num);
                                fwrite(&num, sizeof(num), 1, file2);
                                for (j = 0; j < ng*ngsec; ++j) {
                                        fscanf(file, "%f", &fnum);
                                        dnum = (double    ) fnum;
                                        fwrite(&dnum, sizeof(dnum), 1, file2);
                                }
                        }
                }
	}
	fclose(file);
	fclose(file2);
}
