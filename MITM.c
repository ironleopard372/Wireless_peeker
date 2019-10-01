#include"MITM.h"

int MITM_init(struct MITM *MITM) {
	MITM = malloc(sizeof(struct MITM));
	int exitcode;
	switch (MITM->dev_type) {
	case ethernet :
		break;
	case wireless :
		MITM->l2_packet = l2_packet_init(MITM->monitor_dev, ETH_P_ALL,
						 handle_four_way_shakehand, MITM, 1);
		break;
	}
	if(getifinfo(&(MITM->if_buf), MITM->errbuf)) return 10;
	if(!checkdevice(MITM->if_buf, MITM->usr_dev)) return 11;

	//printf("please type target's ip = ");
	log_printf(MSG_INFO, "please type target's IP = ");
	scanf("%hhd.%hhd.%hhd.%hhd", &(MITM->dev_info.target_ip[0]), &(MITM->dev_info.target_ip[1]),
			&(MITM->dev_info.target_ip[2]), &(MITM->dev_info.target_ip[3]));

	MITM->dev_info.dev = MITM->usr_dev;

	if(exitcode = sniffer_init(&(MITM->dev_info), MITM->errbuf)) return exitcode;

	log_printf(MSG_DEBUG, "sniffer inti successful");

	if(init_hashtable(&MITM->ap_list, 100)) {
		log_printf(MSG_ERROR, "ap list init failed!");
		return -1;
	} else {
		log_printf(MSG_DEBUG, "ap list init successful");
	}
}

int MITM_deinit(struct MITM *MITM) {
	free(MITM->if_buf);
	free(MITM->monitor_buf);
	l2_packet_deinit(MITM->l2_packet);
	free(MITM);
}
