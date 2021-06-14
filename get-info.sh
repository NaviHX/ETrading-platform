cat commdata.txt | \
awk 'BEGIN {print("COMMODITY Info:")} \
!- /^[0123456789]/ {printf("ID: %s\tType: %s\tPrice: %s\tQuant: %s\tOwn: %s\tDesc: %s\tDisc: %s\n",$1,$2,$4,$6,$3,$7,$5)} \
- /^[0123456789]/ {printf("Discount:\nBook: %s\tFood: %s\tCloth: %s\n",$1,$2,$3)}'

cat userdata.txt | \
awk 'BEGIN {print("USER Info:")} \
{printf("ID: %s\tPWD: %s\tBalance: %s\tType: %s\n",$1,$2,$4,$3)}'
