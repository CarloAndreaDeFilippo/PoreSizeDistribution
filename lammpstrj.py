import pandas as pd

ofile = 'configuration.txt'

#Read box size

box_df = pd.read_csv('lammpstrj.txt', skiprows=5, nrows=3, header=None, sep=' ', 
        names=['min', 'max'])

print(box_df)

#Read all particles
All_df = pd.read_csv('lammpstrj.txt', skiprows=9, header=None, sep=' ', 
        names=['id', 'type', 'x', 'y', 'z'])


#Shift particles from [Lmin, Lmax] to [-L/2, L/2]

Lbox = []

for ax in range(3):
    Lbox.append(abs(box_df['max'][ax] - box_df['min'][ax]))

print('Lbox: ', Lbox)

shift = [-0.5 * lbox - lmin for (lbox, lmin) in zip(Lbox, box_df['min'])] 

All_df['x'] += shift[0]
All_df['y'] += shift[1]
All_df['z'] += shift[2]

#Dataframe of polymer and patches
particles_df = All_df[All_df['type'] != 2]
particles_df = particles_df.drop(['id', 'type'], axis=1)

#Dataframe of solvent
solvent_df = All_df[All_df['type'] == 2]
solvent_df = solvent_df.drop(['id', 'type'], axis=1)

print(particles_df)

with open(ofile, 'w') as f:
    f.write(f'{Lbox[0]} {Lbox[1]} {Lbox[2]}\n')

particles_df.to_csv(ofile, header=None, index=None, sep=' ', mode='a')



