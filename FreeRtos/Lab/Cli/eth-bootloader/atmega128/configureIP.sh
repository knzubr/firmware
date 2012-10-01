#!/bin/bash
echo ""
echo "************************************"
echo "*** Bootloader IP configuration: ***"
echo "************************************"
echo ""
echo " " >makefile.in.IP
echo "Set IP addres of bootloader."
echo "Enter 1st octet of addres and press enter:"
read ip1
echo 'EMB_IPADDRESSa = '$ip1 >> makefile.in.IP
echo "Enter 2nd octet of addres and press enter:"
read ip2
echo 'EMB_IPADDRESSb = '$ip2>> makefile.in.IP
echo "Enter 3rd octet of addres and press enter:"
read ip3
echo 'EMB_IPADDRESSc = '$ip3>> makefile.in.IP
echo "Enter 4th octet of addres and press enter:"
read ip4
echo 'EMB_IPADDRESSd = '$ip4>> makefile.in.IP

echo "Thank you, now set IP mask of bootloader"
echo "Enter 1st octet of addres and press enter:"
read mask1
echo 'EMB_IPMASKa = '$mask1 >> makefile.in.IP
echo "Enter 2nd octet of addres and press enter:"
read mask2
echo 'EMB_IPMASKb = '$mask2>> makefile.in.IP
echo "Enter 3rd octet of addres and press enter:"
read mask3
echo 'EMB_IPMASKc = '$mask3>> makefile.in.IP
echo "Enter 4th octet of addres and press enter:"
read mask4
echo 'EMB_IPMASKd = '$mask4>> makefile.in.IP
exit 0
#dodać mac"?
