k5init
tar cvf ndi_distr.tar ndi
echo "Xdiv LAN"
scp ndi_distr.tar xlogin.lanl.gov:/home/release/ndi_distr.tar
echo "LLNL Blue"
scp -oProtocol=1 ndi_distr.tar blue.llnl.gov:release/ndi_distr.tar

