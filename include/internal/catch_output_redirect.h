/*
 *  Created by Martin on 28/04/2018.
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef TWOBLUECUBES_CATCH_OUTPUT_REDIRECT_H
#define TWOBLUECUBES_CATCH_OUTPUT_REDIRECT_H

#include "catch_platform.h"
#include "catch_stream.h"

#include <cstdio>
#include <iosfwd>
#include <string>

namespace Catch {

    class RedirectedStream {
        std::ostream& m_originalStream;
        std::ostream& m_redirectionStream;
        std::streambuf* m_prevBuf;

    public:
        RedirectedStream( std::ostream& originalStream, std::ostream& redirectionStream );
        ~RedirectedStream();
    };

    class RedirectedStdOut {
        ReusableStringStream m_rss;
        RedirectedStream m_cout;
    public:
        RedirectedStdOut();
        auto str() const -> std::string;
    };

    // StdErr has two constituent streams in C++, std::cerr and std::clog
    // This means that we need to redirect 2 streams into 1 to keep proper
    // order of writes
    class RedirectedStdErr {
        ReusableStringStream m_rss;
        RedirectedStream m_cerr;
        RedirectedStream m_clog;
    public:
        RedirectedStdErr();
        auto str() const -> std::string;
    };


    // Windows's implementation of std::tmpfile is terrible (it tries
    // to create a file inside system folder, thus requiring elevated
    // privileges for the binary), so we have to use tmpnam(_s) and
    // create the file ourselves there.
    class TempFile {
    public:
        TempFile(TempFile const&) = delete;
        TempFile& operator=(TempFile const&) = delete;
        TempFile(TempFile&&) = delete;
        TempFile& operator=(TempFile&&) = delete;

        TempFile();
        ~TempFile();

        std::FILE* getFile();
        std::string getContents();

    private:
        std::FILE* m_file = nullptr;
    #if defined(_MSC_VER)
        char m_buffer[L_tmpnam] = { 0 };
    #endif
    };


    class OutputRedirect {
    public:
        OutputRedirect(OutputRedirect const&) = delete;
        OutputRedirect& operator=(OutputRedirect const&) = delete;
        OutputRedirect(OutputRedirect&&) = delete;
        OutputRedirect& operator=(OutputRedirect&&) = delete;


        OutputRedirect(std::string& stdout_dest, std::string& stderr_dest);
        ~OutputRedirect();

    private:
        int m_originalStdout = -1;
        int m_originalStderr = -1;
        TempFile m_stdoutFile;
        TempFile m_stderrFile;
        std::string& m_stdoutDest;
        std::string& m_stderrDest;
    };


} // end namespace Catch

#endif // TWOBLUECUBES_CATCH_OUTPUT_REDIRECT_H
