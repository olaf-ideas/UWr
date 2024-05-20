% Solve 3 document class
% Karol Pokorski

\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{solve}[2013/04/27 Solve 3 document class]

\DeclareOption*{\PassOptionsToClass{\CurrentOption}{article}}
\ProcessOptions\relax
\LoadClass[12pt,a4paper]{article}

\usepackage{polski}
\usepackage[utf8]{inputenc}
\usepackage{amssymb}
\usepackage{geometry}
\usepackage{graphicx}
\usepackage{titlesec}
\usepackage[scaled]{helvet}
\usepackage[T1]{fontenc}
\usepackage{verbatim}

\renewcommand*\familydefault{\sfdefault}

\geometry{verbose,a4paper,tmargin=1cm,bmargin=1cm,lmargin=0.8cm,rmargin=0.8cm}

\pagestyle{empty}

\newcommand{\pageHeader}[4]{
    \raisebox{-0.3cm}{
        \makebox[3.3cm][l]{
            \includegraphics[height=1.2cm]{logo}
        }
    }
    \makebox[10cm][c]{
        \begin{LARGE}\textbf{\textsf{#1}}\end{LARGE}
    }
    \raisebox{0.2cm}{
        \makebox[5cm][r]{
            \begin{tabular}{r l}
                Zadanie: & \textbf{#2} \\
                Limit pamięci: & \textbf{#3 MB} \\
                Limit czasu: & \textbf{#4 s} \\
            \end{tabular}
        }
    }
    \rule{\textwidth}{0.03cm}
    \vspace{-0.7cm}
    \begin{flushright}
      \hspace{0.3cm}
      \includegraphics[height=1.15cm]{power_logo.jpg}
      \hspace{0.3cm}
      \includegraphics[height=1cm]{uwr.png}
      \hspace{0.3cm}
    \end{flushright}
    \vspace{-0.3cm}
}

\newcommand{\testVerb}[3]{
    \begin{minipage}[t]{0.25\linewidth}
        \textbf{Wejście}
        \vspace{-0.3cm}
        \verbatiminput{#1}
    \end{minipage}
    \begin{minipage}[t]{0.25\linewidth}
        \textbf{Wyjście}
        \vspace{-0.3cm}
        \verbatiminput{#2}
    \end{minipage}
    \begin{minipage}[t]{0.5\linewidth}
        #3
    \end{minipage}
    \\[0.4cm]
}

\newcommand{\separator}{
    \\[0.4cm]
}

\titleformat{\section}{\large\scshape\raggedright}{}{0em}{}[\vspace{-0.1cm} \titlerule]
\titleformat{\subsection}{\scshape\raggedright}{}{0em}{\vspace{0.05cm}}[]
