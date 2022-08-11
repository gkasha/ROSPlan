import sys,os

def main(args):
    try:
        planfile = args[0]

        wf = open("./temp.txt", "w")
        with open(planfile, "r") as rf:
            for line in rf:
                if (line[0].isnumeric()):
                    wf.write(line.lower())
        print("----------------------")
        print(planfile)
        wf.close()
        os.remove(planfile)
        os.rename("./temp.txt", planfile.strip(".SOL"))
    except:
        print("PLAN NOT FOUND")
        return

if __name__ == "__main__":
    main(sys.argv[1:])