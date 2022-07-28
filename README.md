# Ebench

Benchmark game engine designs.

## Aunteater benchmark results
|name|iterations|real_time|cpu_time|time_unit|items_per_second|
|----|----------|---------|--------|---------|----------------|
|AddEntity|7448902|71.0164|70.5949|ns|1.41653e+07|
|RemoveEntity 1|7099963|115.304|192.476|ns|8.6727e+06|
|RemoveEntity 16|249010|2038.26|2791.57|ns|7.84984e+06|
|RemoveEntity 32|128601|5985.51|7311.82|ns|5.34625e+06|
|RemoveEntity 64|46007|13386.4|15764.2|ns|4.78097e+06|
|RemoveEntity 128|23494|30800.1|34851.6|ns|4.15582e+06|
|AddAndRemoveEntity 1|6699308|109.295|108.956|ns|9.17805e+06|
|AddAndRemoveEntity 16|292686|2359.67|2351.74|ns|6.80348e+06|
|AddAndRemoveEntity 32|108589|6745.85|6724|ns|4.75907e+06|
|AddAndRemoveEntity 64|46384|15603.8|15555.4|ns|4.11433e+06|
|AddAndRemoveEntity 128|21318|33161.8|33049.8|ns|3.87294e+06|
|AddEntityWithSystem|1000000|609.243|606.703|ns|1.64825e+06|
|SimpleIteration 1|35861331|19.4265|19.3581|ns|5.1658e+07|
|SimpleIteration 1024|44988|14630|14573.7|ns|7.02637e+07|
|SimpleIteration 4096|10144|69713.4|69378.7|ns|5.90383e+07|
|SimpleIteration 32768|100|7.32508e+06|7.27729e+06|ns|4.50278e+06|
|SimpleIteration 131072|26|3.77462e+07|3.75826e+07|ns|3.48757e+06|
|SimpleIteration 1000000|4|2.14742e+08|2.13855e+08|ns|4.67606e+06|
|NestedIterationOnSameArchetype 32|99460|6324.26|6314.68|ns|1.62162e+08|
|NestedIterationOnSameArchetype 64|20339|34686.9|34600.4|ns|1.1838e+08|
|NestedIterationOnSameArchetype 256|857|832801|831712|ns|7.87965e+07|
|NestedIterationOnTwoArchetype 32/2|1489508|467.913|467.171|ns|1.36995e+08|
|NestedIterationOnTwoArchetype 64/2|736940|990.577|989.249|ns|1.29391e+08|
|NestedIterationOnTwoArchetype 256/2|161498|4366.92|4358.19|ns|1.1748e+08|
|NestedIterationOnTwoArchetype 32/8|604353|1143.1|1141.41|ns|2.24285e+08|
|NestedIterationOnTwoArchetype 64/8|305971|2253.54|2250.72|ns|2.27483e+08|
|NestedIterationOnTwoArchetype 256/8|59709|11850.4|11833.3|ns|1.73071e+08|
|NestedIterationOnTwoArchetype 32/16|260439|2464.51|2461.27|ns|2.08023e+08|
|NestedIterationOnTwoArchetype 64/16|127865|5140.31|5132.27|ns|1.99522e+08|
|NestedIterationOnTwoArchetype 256/16|30370|25545.3|25498.2|ns|1.60639e+08|
|SimpleIterationOnVaryingSize 1|260|2.77839e+06|2.76822e+06|ns|3.61244e+06|
|SimpleIterationOnVaryingSize 2|295|2.73227e+06|2.70475e+06|ns|3.69719e+06|
|SimpleIterationOnVaryingSize 4|290|2.7472e+06|2.73984e+06|ns|3.64985e+06|
|SimpleIterationOnVaryingSize 8|442|1.7691e+06|1.76485e+06|ns|5.66622e+06|
|SimpleIterationOnVaryingSize 16|279|2.47458e+06|2.46727e+06|ns|4.05307e+06|
|AddComponent|17280874|39.7581|39.709|ns|2.51832e+07|
|RemoveComponent 1|3770824|183.856|458.413|ns|5.43904e+06|
|RemoveComponent 16|372012|1917.12|4245.43|ns|8.34584e+06|
|RemoveComponent 32|169358|4104.91|9470.36|ns|7.79555e+06|
|RemoveComponent 64|77118|8890.1|21462.3|ns|7.19902e+06|
|RemoveComponent 128|31901|22859.1|56944.3|ns|5.59951e+06|
|AddAndRemoveComponent 1|1798531|398.244|396.106|ns|2.52458e+06|
|AddAndRemoveComponent 16|168439|4127.05|4113.28|ns|3.88984e+06|
|AddAndRemoveComponent 32|71720|9300.43|9263.82|ns|3.4543e+06|
|AddAndRemoveComponent 64|32742|23700.7|23534.1|ns|2.71946e+06|
|AddAndRemoveComponent 128|10127|60172.1|58772.3|ns|2.1779e+06|

## Entity benchmark results
|name|iterations|real_time|cpu_time|time_unit|items_per_second|
|----|----------|---------|--------|---------|----------------|
|AddEntity|2236009|347.758|347.454|ns|2.87808e+06|
|RemoveEntity 1|1245742|797.965|1052.42|ns|1.25319e+06|
|RemoveEntity 16|84804|10539.6|13751.1|ns|1.51809e+06|
|RemoveEntity 32|42592|21467.6|27919.7|ns|1.49062e+06|
|RemoveEntity 64|19489|39996.4|52609.1|ns|1.60014e+06|
|RemoveEntity 128|12990|79374.1|105303|ns|1.61262e+06|
|AddAndRemoveEntity 1|965474|887.89|887.182|ns|1.12716e+06|
|AddAndRemoveEntity 16|66442|13947.9|13927.7|ns|1.14879e+06|
|AddAndRemoveEntity 32|32071|25890.8|25856.1|ns|1.23762e+06|
|AddAndRemoveEntity 64|15220|49644.6|49612.3|ns|1.29e+06|
|AddAndRemoveEntity 128|9301|101449|101304|ns|1.26353e+06|
|SimpleIteratio 1|122415980|5.51399|5.51091|ns|1.81458e+08|
|SimpleIteratio 1024|6128181|97.4622|97.371|ns|1.05165e+10|
|SimpleIteratio 4096|1963562|365.937|365.563|ns|1.12046e+10|
|SimpleIteratio 32768|127650|4172.38|4161.1|ns|7.87484e+09|
|SimpleIteratio 131072|37016|19722|19696.2|ns|6.65468e+09|
|SimpleIteratio 1000000|3348|195482|195084|ns|5.126e+09|
|NestedIterationOnSameArchetyp 32|456373|1483.7|1482.88|ns|6.9055e+08|
|NestedIterationOnSameArchetyp 64|99348|7101.88|7096.52|ns|5.77185e+08|
|NestedIterationOnSameArchetyp 256|5952|118122|118055|ns|5.5513e+08|
|NestedIterationOnTwoArchetyp 32/2|3692450|190.351|190.201|ns|3.36487e+08|
|NestedIterationOnTwoArchetyp 64/2|1911146|366.553|366.329|ns|3.49412e+08|
|NestedIterationOnTwoArchetyp 256/2|498064|1404.92|1404.01|ns|3.6467e+08|
|NestedIterationOnTwoArchetyp 32/8|3103491|199.685|199.565|ns|1.28279e+09|
|NestedIterationOnTwoArchetyp 64/8|1596443|391.601|391.313|ns|1.30841e+09|
|NestedIterationOnTwoArchetyp 256/8|463711|1513.6|1511.93|ns|1.35456e+09|
|NestedIterationOnTwoArchetyp 32/16|2859092|265.041|264.904|ns|1.93277e+09|
|NestedIterationOnTwoArchetyp 64/16|1479181|474.564|474.231|ns|2.15928e+09|
|NestedIterationOnTwoArchetyp 256/16|388880|1811.74|1810.35|ns|2.26254e+09|
|SimpleIterationOnVaryingSize 1|579012|1196.72|1196.07|ns|8.36072e+09|
|SimpleIterationOnVaryingSize 2|301746|2315.2|2314.01|ns|4.32151e+09|
|SimpleIterationOnVaryingSize 4|146374|4849.39|4845.73|ns|2.06367e+09|
|SimpleIterationOnVaryingSize 8|65351|10683|10675.9|ns|9.36686e+08|
|SimpleIterationOnVaryingSize 16|30850|22501.4|22484.4|ns|4.44753e+08|
|AddComponent|2613936|273.377|273.197|ns|3.66037e+06|
|RemoveComponent 1|210242|3725.55|4703.4|ns|268417|
|RemoveComponent 16|5822|124248|127289|ns|128775|
|RemoveComponent 32|1119|634723|640609|ns|50415.7|
|RemoveComponent 64|203|3.58776e+06|3.59576e+06|ns|17838.5|
|RemoveComponent 128|32|2.11629e+07|2.11755e+07|ns|6048.31|
|AddAndRemoveComponent 1|172204|4562.95|4559.48|ns|219323|
|AddAndRemoveComponent 16|5726|126270|126127|ns|126856|
|AddAndRemoveComponent 32|1091|638826|638225|ns|50139|
|AddAndRemoveComponent 64|203|3.47107e+06|3.46722e+06|ns|18458.6|
|AddAndRemoveComponent 128|33|2.1207e+07|2.11701e+07|ns|6046.26|

## Flecs benchmark results
|name|iterations|real_time|cpu_time|time_unit|items_per_second|
|----|----------|---------|--------|---------|----------------|
|AddEntity|11207024|63.3859|63.2912|ns|1.58e+07|
|RemoveEntity 1|12085190|60.4423|161.383|ns|1.65447e+07|
|RemoveEntity 16|847783|825.446|1964.52|ns|1.93835e+07|
|RemoveEntity 32|501630|1386.97|3258|ns|2.30718e+07|
|RemoveEntity 64|200154|3521.93|8035.03|ns|1.81719e+07|
|RemoveEntity 128|81406|6439.04|14646.4|ns|1.98787e+07|
|AddAndRemoveEntity 1|6521299|116.314|116.089|ns|8.61405e+06|
|AddAndRemoveEntity 16|423398|1851.06|1846.41|ns|8.66545e+06|
|AddAndRemoveEntity 32|177210|3695.07|3687.66|ns|8.67759e+06|
|AddAndRemoveEntity 64|89326|8695.95|8610.17|ns|7.43307e+06|
|AddAndRemoveEntity 128|43811|13189.9|13153.1|ns|9.73151e+06|
|SimpleIteration 1|6168838|149.543|144.975|ns|6.89774e+06|
|SimpleIteration 1024|2646987|257.715|257.258|ns|3.98044e+09|
|SimpleIteration 4096|894839|976.245|933.401|ns|4.38825e+09|
|SimpleIteration 32768|97461|6839.71|6548.25|ns|5.00409e+09|
|SimpleIteration 131072|29249|24084.2|23890.6|ns|5.48634e+09|
|SimpleIteration 1000000|1812|580565|506838|ns|1.97302e+09|
|NestedIterationOnSameArchetyp 32|112048|6813.18|6799.98|ns|1.50589e+08|
|NestedIterationOnSameArchetyp 64|46456|17200.5|16921.7|ns|2.42056e+08|
|NestedIterationOnSameArchetyp 256|4467|188525|174730|ns|3.7507e+08|
|NestedIterationOnTwoArchetyp 32/2|108466|6820.74|6175.73|ns|1.03631e+07|
|NestedIterationOnTwoArchetyp 64/2|56736|9051.05|8879.04|ns|1.4416e+07|
|NestedIterationOnTwoArchetyp 256/2|22825|29693.9|29611.6|ns|1.72905e+07|
|NestedIterationOnTwoArchetyp 32/8|176944|5441.43|5151.6|ns|4.96933e+07|
|NestedIterationOnTwoArchetyp 64/8|54665|14483|12652.9|ns|4.0465e+07|
|NestedIterationOnTwoArchetyp 256/8|14270|43250.1|41852.4|ns|4.89339e+07|
|NestedIterationOnTwoArchetyp 32/16|158787|4317.89|4283.49|ns|1.19529e+08|
|NestedIterationOnTwoArchetyp 64/16|68098|9500.32|9475.55|ns|1.08068e+08|
|NestedIterationOnTwoArchetyp 256/16|21499|46404.2|44242.3|ns|9.2581e+07|
|SimpleIterationOnVaryingSize 1|43422|14827|14770.7|ns|6.77015e+08|
|SimpleIterationOnVaryingSize 2|58655|10957.7|10842.1|ns|9.2233e+08|
|SimpleIterationOnVaryingSize 4|69822|10126.1|10102.4|ns|9.89863e+08|
|SimpleIterationOnVaryingSize 8|67757|10249.3|10155.7|ns|9.84667e+08|
|SimpleIterationOnVaryingSize 16|73893|10001.3|9951.08|ns|1.00492e+09|
|AddComponent|12583786|73.2095|71.2912|ns|1.4027e+07|
|RemoveComponent 1|5466680|149.944|640.93|ns|6.66915e+06|
|RemoveComponent 16|477205|1495.41|10367.5|ns|1.06994e+07|
|RemoveComponent 32|204021|2671.77|13657.6|ns|1.19771e+07|
|RemoveComponent 64|148742|4907.48|34840.2|ns|1.30413e+07|
|RemoveComponent 128|86742|7923.8|83097|ns|1.61539e+07|
|AddAndRemoveComponent 1|1352832|501.747|500.312|ns|1.99875e+06|
|AddAndRemoveComponent 16|101588|6807.48|6786.97|ns|2.35746e+06|
|AddAndRemoveComponent 32|75307|8914.77|8891.2|ns|3.59906e+06|
|AddAndRemoveComponent 64|27192|32596.4|32265.2|ns|1.98356e+06|
|AddAndRemoveComponent 128|6964|101828|100012|ns|1.27985e+06|
