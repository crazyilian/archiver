#include "CommandLineArguments.h"
#include "Compress.h"
#include "Decompress.h"
#include "Exceptions.h"
#include "PrintUtils.h"

int main(int argc, char **argv) {
    CommandLineArguments args(argc, argv);
    try {
        if (args.IsOptionExists("h") || args.GetOptionsNumber() == 0) {  // archiver -h    // archiver
            PrintHelpMessage();
        } else if (args.GetOptionsNumber() != 1 || !args.GetValuesWithNoOption().empty()) {
            throw InvalidNumberOfArguments();
        } else if (args.IsOptionExists("c")) {  // archiver -c archive_name file1 [file2 ...]
            Compress(args.GetValuesByOption("c"));
        } else if (args.IsOptionExists("d")) {  // archiver -d archive_name
            Decompress(args.GetValuesByOption("d"));
        } else {
            throw InvalidOption(args.GetOptions()[0]);
        }
    } catch (InvalidNumberOfArguments &e) {
        PrintMessageAndTryHelp(e.what());
        return 111;
    } catch (InvalidOption &e) {
        PrintMessageAndTryHelp(e.what());
        return 111;
    } catch (MessageException &e) {
        PrintMessage(e.what());
        return 111;
    }
    return 0;
}
