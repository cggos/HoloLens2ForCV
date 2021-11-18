"""
1. get kitti format file after running the script
2. plot:
        evo_traj  kitti <path-to-rig2world.kitti> -p --plot_mode xz
"""
import argparse


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--dir", required=True, help="the dir of xxx_rig2world.txt")
    args = parser.parse_args()

    data_dir = args.dir + "/"

    of_rig2world = open(data_dir + "Depth Long Throw_rig2world.txt")

    of_kitti = open(data_dir + "rig2world.kitti", 'w')

    for line in of_rig2world:
        data = line.split(',')
        assert len(data) == 17
        for i in range(1, 13):
            if i == 12:
                of_kitti.write("{}\n".format(float(data[12])))
                break
            of_kitti.write("{} ".format(float(data[i])))

    of_kitti.close()


if __name__ == "__main__":
    main()
