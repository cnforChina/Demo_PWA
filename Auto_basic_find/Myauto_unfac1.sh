#!/bin/bash
##The example in this file is jpsi->gamma eta pi0 pi0
##Write by xiongxa and xulei
##
##This script need:
## 1.change the links in this file to yours
## 2.change the name of the partial waves to yours
## 3.make the partial waves are corresponding to your partial wave analysis function
## 4.change the number of degree of freedom to yours
## 5.source root before run this script
## 6.using nohup bash Myauto_unfac1.sh & to put the script into backend
##----------------The candidate partial waves are list in the wave_tot list.-------##
wave_tot=(
X2370_1pp_a0980
X2370_0mp_a0980 X2370_0mp_a21320 X2370_0mp_a21700 X2370_0mp_a0phsp X2370_0mp_a2phsp 
X2370_0mp_f01500 X2370_0mp_f01710 X2370_0mp_f2phsp X2370_0mp_f21270 X2370_0mp_f21525
X2370_0mp_f0phsp X2370_0mp_f0500 X2370_0mp_f0980 X2370_0mp_f01370 
X2370_0mp_a01450 X2370_0mp_a01950

X2370_1pp_a0980 X2370_1pp_a0phsp X2370_1pp_a21320 X2370_1pp_a21700 X2370_1pp_a2phsp 
X2370_1pp_f01500 X2370_1pp_f01710 X2370_1pp_f2phsp X2370_1pp_f21270 X2370_1pp_f21525
X2370_1pp_f0phsp X2370_1pp_f0500 X2370_1pp_f0980 X2370_1pp_f01370 
X2370_1pp_a01450 X2370_1pp_a01950

X2370_2pp_a21320 X2370_2pp_a21700 X2370_2pp_a2phsp X2370_2pp_f2phsp X2370_2pp_f21270 
X2370_2pp_f21525

X2370_2mp_a0980 X2370_2mp_a21320 X2370_2mp_a21700 X2370_2mp_a0phsp X2370_2mp_a2phsp 
X2370_2mp_f01500 X2370_2mp_f01710 X2370_2mp_f2phsp X2370_2mp_f21270 X2370_2mp_f21525
X2370_2mp_f0phsp X2370_2mp_f0500 X2370_2mp_f0980 X2370_2mp_f01370 
X2370_2mp_a01450 X2370_2mp_a01950

Xphsp_0mp_a0phsp Xphsp_0mp_a0980 Xphsp_0mp_a2phsp Xphsp_0mp_a21320 Xphsp_0mp_a21700
Xphsp_0mp_f01500 Xphsp_0mp_f01710 Xphsp_0mp_f2phsp Xphsp_0mp_f21270 Xphsp_0mp_f21525
Xphsp_0mp_f0phsp Xphsp_0mp_f0500 Xphsp_0mp_f0980 Xphsp_0mp_f01370 
Xphsp_0mp_a01450 Xphsp_0mp_a01950

X2120_0mp_a0980 X2120_0mp_a21320 X2120_0mp_a21700 X2120_0mp_a0phsp X2120_0mp_a2phsp 
X2120_0mp_f01500 X2120_0mp_f01710 X2120_0mp_f2phsp X2120_0mp_f21270 X2120_0mp_f21525
X2120_0mp_f0phsp X2120_0mp_f0500 X2120_0mp_f0980 X2120_0mp_f01370
X2120_0mp_a01450 X2120_0mp_a01950

X1835_0mp_a0phsp X1835_0mp_a0980 X1835_0mp_a2phsp X1835_0mp_a21320 X1835_0mp_a21700
X1835_0mp_f01500 X1835_0mp_f01710 X1835_0mp_f2phsp X1835_0mp_f21270 X1835_0mp_f21525
X1835_0mp_f0phsp X1835_0mp_f0500 X1835_0mp_f0980 X1835_0mp_f01370
X1835_0mp_a01450 X1835_0mp_a01950

Xphsp_1pp_a0phsp Xphsp_1pp_a0980 Xphsp_1pp_a2phsp Xphsp_1pp_a21320 Xphsp_1pp_a21700
Xphsp_1pp_f01500 Xphsp_1pp_f01710 Xphsp_1pp_f2phsp Xphsp_1pp_f21270 Xphsp_1pp_f21525
Xphsp_1pp_f0phsp Xphsp_1pp_f0500 Xphsp_1pp_f0980 Xphsp_1pp_f01370
Xphsp_1pp_a01450 Xphsp_1pp_a01950

Xphsp_2mp_a0phsp Xphsp_2mp_a0980 Xphsp_2mp_a2phsp Xphsp_2mp_a21320 Xphsp_2mp_a21700
Xphsp_2mp_f01500 Xphsp_2mp_f01710 Xphsp_2mp_f2phsp Xphsp_2mp_f21270 Xphsp_2mp_f21525
Xphsp_2mp_f0phsp Xphsp_2mp_f0500 Xphsp_2mp_f0980 Xphsp_2mp_f01370
Xphsp_2mp_a01450 Xphsp_2mp_a01950

Xphsp_2pp_a2phsp Xphsp_2pp_a21320 Xphsp_2pp_a21700 Xphsp_2pp_f2phsp Xphsp_2pp_f21270
Xphsp_2pp_f21525

)

##----------------The candidate partial waves are list in the wave_tot list.-------##
dof_tot=(
4
2 2 2 2 2
2 2 2 2 2
2 2 2 2 
2 2

4 4 8 8 8
4 4 8 8 8
4 4 4 4 
4 4

6 6 6 6 6
6

6 6 6 6 6
6 6 6 6 6
6 6 6 6 
6 6

2 2 2 2 2
2 2 2 2 2
2 2 2 2 
2 2

2 2 2 2 2
2 2 2 2 2
2 2 2 2 
2 2

2 2 2 2 2
2 2 2 2 2
2 2 2 2 
2 2

4 4 8 8 8
4 4 8 8 8
4 4 4 4 
4 4

6 6 6 6 6
6 6 6 6 6
6 6 6 6 
6 6

6 6 6 6 6
6
)

##---initial basic solution
wave_ini=(
X1835_0mp_a0980 X1835_0mp_a21320 X1835_0mp_f0980 X1835_0mp_f21270 X1835_0mp_f0500
X2370_0mp_f2phsp Xphsp_0mp_a0phsp Xphsp_0mp_a0980 Xphsp_0mp_a21320 Xphsp_0mp_f01370
Xphsp_0mp_f01500 X2370_0mp_f21270 Xphsp_0mp_f2phsp Xphsp_0mp_a21700 Xphsp_0mp_f01710
X2370_0mp_f01710 X2370_0mp_a0980 Xphsp_0mp_f21270 Xphsp_0mp_f0980 Xphsp_0mp_f0500
Xphsp_0mp_a2phsp X2370_0mp_a01450
)
##---initial basic number of dgree of freedom
dof_ini=(
    2 2 2 2 2
    2 2 2 2 2
    2 2 2 2 2
    2 2 2 2 2
    2 2
)

like_0="0"  ###### if the like_0 is 0,  the initial check_end should be set ${#wave_ini[@]}

mcjobprefix='J' ### just for convenience to change
fitjobprefix='K'
checkjobprefix='L'

#part of adding one wave into the ini array 
#gpuprogram="/besfs/groups/gpugroup/xulei/GPUPWA2.1new/gpupwa_mtj/GEPP_unfact/GEtaPi0Pi0"
gpuMCInteprogram="/besfs/groups/psip/psipgroup/user/xiongxa/GamEtaPi0Pi0/Alg_gg_xl/GPUPWA/gpupwa2.1_1/GEPP_unfact_mcinte_xxa/GEtaPi0Pi0"
gpuFitprogram="/besfs/groups/psip/psipgroup/user/xiongxa/GamEtaPi0Pi0/Alg_gg_xl/GPUPWA/gpupwa2.1_1/GEPP_unfact_onlyFit_xxa/GEtaPi0Pi0"
binfilesdir="/besfs/groups/psip/psipgroup/user/xiongxa/GamEtaPi0Pi0/Alg_gg_xl/GPUPWA/gpupwa2.1_1/GPUPWA/_common/binfiles"

resfile="/besfs/groups/psip/psipgroup/user/xiongxa/GamEtaPi0Pi0/Alg_gg_xl/GPUPWA/gpupwa2.1_1/GEPP_unfact_mcinte_xxa/input/res.inp"
inputfiles="/besfs/groups/psip/psipgroup/user/xiongxa/GamEtaPi0Pi0/Alg_gg_xl/GPUPWA/gpupwa2.1_1/Bin_Bin_more_phsp/mypwa/files.txt"

path=$PWD
fitjobnums=10  ## The number of job in each fit
N5sigma=1
echo "lets begin!!"  `date` > Bin_Bin_Fit.log
echo "begin" `date` > sucess.out

runs=0   #### count the adds
checkruns=0 ###### count the checks

add_n=0
while [ "$add_n" -lt "${#wave_tot[@]}" ]  ## Mian loop, loop all partial waves 
do
  read -r line < fail.inp  ##check the fail.inp to see if stop the loop
  if [ "$line"x = "stop"x ];then
    echo "!!!!!!!!!!!!!!!!!!!stop by $USER!!!!!!!!!!!!!!!!!!!!" `date` >>Bin_Bin_Fit.log
    break
  fi

## check if the wave is already contained in the basic solution, if not, try
  add_stat=1
  for j in "${!wave_ini[@]}"; do 
    if [ "${wave_ini[$j]}"x = "${wave_tot[$add_n]}"x ];then
      add_stat=0
    fi 
  done 
  if [ $add_stat -eq 0 ];then
    let add_n++
    continue
  fi

  echo "Before adding,the likeli value is $like_0 and the solution is ${wave_ini[@]}" >> Bin_Bin_Fit.log
  wave_ini[${#wave_ini[@]}]="${wave_tot[$add_n]}"
  dof_ini[${#dof_ini[@]}]="${dof_tot[$add_n]}" 

## make dir and jobs for this try
  let runs++
  mkdir -p  add_${wave_tot[$add_n]}_$runs
  cd  add_${wave_tot[$add_n]}_$runs 
  ln -s $gpuMCInteprogram  ./MCInte_GEtaPi0Pi0
  ln -s  $binfilesdir   ./
  mkdir  input
  cp $inputfiles  input/files.txt
  cp $resfile   input/

  for i in "${!wave_ini[@]}"; do
    echo ${wave_ini[$i]} >> input/wave.inp
  done
  ##################################### MC Integral
  $path/Source/ConvConfig_xxa2xl.pl  input/wave.inp  input/fileconfig.txt
  $path/Source/GenParIniRan_unfac.py input/fileconfig.txt  input/para.inp
  $path/Source/GenJob2par.sh  ${mcjobprefix}_${runs}_  MCInte_GEtaPi0Pi0
  gpu2q   ${mcjobprefix}_${runs}_
  checkmctimes=0
  while true
  do
    jobflag=`grep 'End===End'  MCInte_GEtaPi0Pi0.log`
    if [ "$jobflag" != "" ];then
      break
    fi
    sleep 30
    let checkmctimes+=1
    if [ "$checkmctimes" -gt  240 ];then  #Time for MC Integral is 2h
      echo "Something is wrong! MCInte Terminaled " >>$path/Bin_Bin_Fit.log
      exit -1
    fi

    read -r line < $path/fail.inp
    if [ "$line"x = "stop"x ];then   #Stop the program by user
      echo "!!!!!!!!!!!!!!!!!!!stop by $USER!!!!!!!!!!!!!!!!!!!!" `date` >>$path/Bin_Bin_Fit.log
#      exit -1
      break
    fi
  done

  for k in `seq 1 $fitjobnums`
  do
    mkdir Fit_$k  -p
    cd Fit_$k
    ln -s $gpuFitprogram  ./Fit_GEtaPi0Pi0
    ln -s  $binfilesdir   ./
    mkdir  input
    cd input
    cp  -s  ../../input/files.txt   .
    cp -s  ../../input/res.inp   .
    cp -s  ../../input/fileconfig.txt  .
    $path/Source/GenParIniRan_unfac.py  fileconfig.txt  para.inp
    cd ..
    cp -s  ../GammaEtaPi0Pi0Analysis_Amplitude_MC_Integral    .
    $path/Source/GenJob2par.sh  ${fitjobprefix}_${runs}_$k  Fit_GEtaPi0Pi0
    gpu2q ${fitjobprefix}_${runs}_$k
    cd ..
  done

  $path/Source/checkgpujob.sh   ${fitjobprefix}_${runs}_  #Time for each fit job is 6h
  ################################ check significance for this wave
  like_add_test=`cat Fit_*/multifitresults_GammaEtaPi0Pi0Analysis_*.txt |awk '{print $6}'|sed 's#:##'|sed '/^$/d'|sort -nr|tail -n1`
  like_add=`cat Fit_*/multifitresults_GammaEtaPi0Pi0Analysis_*.txt |awk '{print $6}'|sed 's#:##'|sed '/^$/d'|sort -n|$path/Source/check_conv_simple2.pl`
  if [ "$like_add" != "$like_add_test" ];then
    echo "the likelihood is strange, the least $like_add_test is abandoned! use $like_add ">>$path/Bin_Bin_Fit.log
    echo "This likelihood is stroed in file Strange_lik.log,marked as $like_add_test." >>$path/Bin_Bin_Fit.log    #xiongxa
    echo "*********************$like_add_test************************" >>$path/Strange_lik.log #xiongxa
    cat Fit_*/multifitresults_GammaEtaPi0Pi0Analysis_*.txt |awk '{print $6}'|sed 's#:##'|sed '/^$/d'|sort -nr >>$path/Strange_lik.log #xiongxa
  fi
  if [ "$like_add" = "" ];then
    echo Something is wrong! >>$path/Bin_Bin_Fit.log
    break
  fi
  cd  $path
  value3=`echo $like_0 - $like_add|bc`
  sigf=`/workfs/bes/xulei/significance/significance $value3  ${dof_tot[$add_n]}|perl -nle 'print $1 if(/significance= *(\S+)/)'`
  pmflag=`echo $value3|perl -nle 'if($_>=0){print 1}else{print 0}'`
  flag=`echo $sigf|perl -nle 'if($_<5){print 0}else{print 1}'`
  if [ "$pmflag" = 0 ];then
    flag=0
  fi
  echo "trying to add wave ${wave_tot[$add_n]},the new like value of new likelihood is $like_add ,the signif is  $sigf" >> Bin_Bin_Fit.log
#  rm  $path/add_${wave_tot[$add_n]}_$runs -fr  # we want to save the file 

  if [ "$flag"x = "0"x ];then
    echo "So the wave ${wave_ini[${#wave_ini[@]}-1]} dont add, try next wave" >> Bin_Bin_Fit.log
    add_tem=${#wave_ini[@]}-1
    wave_ini=(${wave_ini[@]:0:$add_tem} ${wave_ini[@]:$add_tem+1})
    dof_ini=(${dof_ini[@]:0:$add_tem} ${dof_ini[@]:$add_tem+1})
    let add_n++
    rm  $path/add_${wave_tot[$add_n]}_$runs -fr  # we discard the file which is less than 5sigma
    continue
  fi


  like_0=$like_add
  echo  >>Bin_Bin_Fit.log
  echo "So the wave ${wave_ini[${#wave_ini[@]}-1]} have added, next part is to check the sig for everyone in ${#wave_ini[@]} waves" >> Bin_Bin_Fit.log

#save the new combination,if the check part find that baisc componants are lager than 5sigma, other componants are less than 5sigma, this is the final file we need!!!
  echo "New file add_succeed is updated!!!" >> Bin_Bin_Fit.log
  rm -rf $path/add_succeed
  mv $path/add_${wave_tot[$add_n]}_$runs $path/add_succeed 
#  echo "break by temporary!!!" >> Bin_Bin_Fit.log
#  break #for temporary, find the basic ....
  #########################################part of checking the sig. for every one inside 

  like_1="0"
  check_end=${#wave_ini[@]}
# let check_end-=1   ################################   this line should be deleted if you set like_0=0
  while true  ###check until all 5sigma
  do   ###### DeadRun1111111 start
    cd $path
    check_run_flag=0  #### denote the change of status 
    let checkruns++
    read -r line < $path/fail.inp
    if [ "$line"x = "stop"x ];then
      echo "!!!!!!!!!!!!!!!!!!!stop by $USER!!!!!!!!!!!!!!!!!!!!" `date` >>Bin_Bin_Fit.log
#      exit -1
      break  #xiongxa break to keep everything unchange when stop
    fi

    if [ ${#wave_ini[@]} -eq 1 ];then
      break
    fi

    check_n=0  ################################## check_n can be set as check_n=n, if the previous n states don't need to be checked
    while [ $check_n -lt $check_end ]
    do   ###### checkrun=====start

      ####### jump out one check run
      if [ "$check_run_flag" = 1 ];then
	      break
      fi

      cd $path
      read -r line < $path/fail.inp
      if [ "$line"x = "stop"x ];then
	      echo "!!!!!!!!!!!!!!!!!!!stop by $USER!!!!!!!!!!!!!!!!!!!!" `date` >>Bin_Bin_Fit.log
	      break
      fi
      mkdir C_${checkruns}_$check_n  -p
      cd C_${checkruns}_$check_n

      ln -s $gpuMCInteprogram  ./MCInte_GEtaPi0Pi0
      ln -s  $binfilesdir   ./
      mkdir  input
      cp $inputfiles  input/files.txt
      cp $resfile   input/

      for i in "${!wave_ini[@]}"; do
	      if [ $i -eq $check_n ];then
	        continue
	      fi
	      echo ${wave_ini[$i]} >> input/wave.inp
      done
      $path/Source/ConvConfig_xxa2xl.pl  input/wave.inp  input/fileconfig.txt
      $path/Source/GenParIniRan_unfac.py input/fileconfig.txt  input/para.inp
      $path/Source/GenJob2par.sh  ${checkjobprefix}_M${checkruns}_${check_n}  MCInte_GEtaPi0Pi0
      gpu2q   ${checkjobprefix}_M${checkruns}_${check_n}
      checkmctimes=0
      while true
      do
	      jobflag=`grep 'End===End'  MCInte_GEtaPi0Pi0.log`
	      if [ "$jobflag" != "" ];then
	        break
	      fi
	      sleep 30
	      let checkmctimes+=1
	      if [ "$checkmctimes" -gt  240 ];then
	        echo "Something is wrong! MCInte Terminaled " >>$path/Bin_Bin_Fit.log
	        exit -1
	      fi
	      read -r line < $path/fail.inp
	      if [ "$line"x = "stop"x ];then
	        echo "!!!!!!!!!!!!!!!!!!!stop by $USER!!!!!!!!!!!!!!!!!!!!" `date` >>Bin_Bin_Fit.log
#	        exit -1
          break
	      fi
      done

      for k in `seq 1 $fitjobnums`
      do
	      mkdir Fit_$k  -p
	      cd Fit_$k
	      ln -s $gpuFitprogram  ./Fit_GEtaPi0Pi0
	      ln -s  $binfilesdir   ./
	      mkdir  input
	      cd input
	      cp  -s  ../../input/files.txt   .
	      cp -s  ../../input/res.inp   .
	      cp -s  ../../input/fileconfig.txt  .
	      $path/Source/GenParIniRan_unfac.py  fileconfig.txt  para.inp
	      cd ..
	      cp -s  ../GammaEtaPi0Pi0Analysis_Amplitude_MC_Integral    .
	      $path/Source/GenJob2par.sh  ${checkjobprefix}_${checkruns}_${check_n}_$k  Fit_GEtaPi0Pi0
	      gpu2q  ${checkjobprefix}_${checkruns}_${check_n}_$k
	      cd ..
      done
      $path/Source/checkgpujob.sh   ${checkjobprefix}_${checkruns}_${check_n}_

      like_1_test=`cat Fit_*/multifitresults_GammaEtaPi0Pi0Analysis_*.txt |awk '{print $6}'|sed 's#:##'|sed '/^$/d'|sort -nr|tail -n1`
      like_1=`cat Fit_*/multifitresults_GammaEtaPi0Pi0Analysis_*.txt |awk '{print $6}'|sed 's#:##'|sed '/^$/d'|sort -nr|$path/Source/check_conv_simple2.pl`
      if [ "$like_1" != "$like_1_test" ];then
	      echo "In check,the likelihood is strange, the least $like_1_test is abandoned! use $like_1 ">>$path/Bin_Bin_Fit.log
      fi
      if [ "$like_1" = "" ];then
	      echo  in check Something is wrong! set it to be 0 >>$path/Bin_Bin_Fit.log
	      like_1=0
      fi

      cd $path
      value3=`echo $like_1 - $like_0|bc`
      sigf=`/workfs/bes/xulei/significance/significance $value3  ${dof_ini[$check_n]}|perl -nle 'print $1 if(/significance= *(\S+)/)'`
      pmflag=`echo $value3|perl -nle 'if($_>=0){print 1}else{print 0}'`
      flag=`echo $sigf|perl -nle 'if($_<5){print 0}else{print 1}'`
      if [ "$pmflag" = 0 ];then
	      flag=0
      fi

      if [ "$flag"x = "0"x ];then
	      echo "wave ${wave_ini[$check_n]} have checked with ${#wave_ini[@]} waves" >> Bin_Bin_Fit.log
	      echo "Before drop the wave ${wave_ini[$check_n]},the like value is $like_0" >> Bin_Bin_Fit.log
	      echo "After drop the wave ${wave_ini[$check_n]},the like value is $like_1" >> Bin_Bin_Fit.log
	      echo "So the wave ${wave_ini[$check_n]} will be dropped, the significance is $sigf" >> Bin_Bin_Fit.log

	      wave_ini=(${wave_ini[@]:0:$check_n} ${wave_ini[@]:$check_n+1})
	      dof_ini=(${dof_ini[@]:0:$check_n} ${dof_ini[@]:$check_n+1})
	      like_0=$like_1 #change the base solution value 
	      echo "After the wave is droped and the base value should change to $like_0" >> Bin_Bin_Fit.log
	      check_run_flag=1
	      check_end=${#wave_ini[@]}
        #save new file, if new basic componants are lager than 5sigma and the other componants are all less than 5sigma, this is the final file we need!!!!
        rm -rf $path/add_succeed
        cp -rf $path/C_${checkruns}_$check_n $path/add_succeed
	      break
      fi

      echo "wave ${wave_ini[$check_n]} have checked with ${#wave_ini[@]} waves" >> Bin_Bin_Fit.log
      echo "Before drop the wave ${wave_ini[$check_n]},the like value is $like_0" >> Bin_Bin_Fit.log
      echo "After drop the wave ${wave_ini[$check_n]},the like value is $like_1, the significance is $sigf, wave remain " >> Bin_Bin_Fit.log
      let check_n++

    done ###### checkrun=====end

    cd $path
    rm  -fr  C_${checkruns}_* 


    ######### jump out of check loop
    if [ "$check_run_flag" = 0 ];then
      echo "Now left ${#wave_ini[@]} waves(> 5sigma)" >> Bin_Bin_Fit.log
      echo  >> Bin_Bin_Fit.log
      break
    fi
  done #######DeadRun111111111 end

  echo "Now $N5sigma added are sig. > 5sigma " `date` >> sucess.out
  echo "likelihood value is $like_0" >> sucess.out
  for i in "${!wave_ini[@]}"; do
    echo ${wave_ini[$i]}    ${dof_ini[$i]} >> sucess.out
  done
  echo  >>sucess.out
  let N5sigma++

  let add_n=0 #restart the adding part from the first particle
done

echo "======= End" >>Bin_Bin_Fit.log
