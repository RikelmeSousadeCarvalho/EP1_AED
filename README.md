# EP1_AED

Planilhas eletrônicas, como o Microsoft Excel ou o Google Sheets, organizam dados em
grades bidimensionais e estão presentes no dia a dia de praticamente qualquer área, da
engenharia às ciências humanas. Na prática, no entanto, a grande maioria das células de
uma planilha permanece vazia – ou, no contexto deste EP, com o valor 0. Armazenar
todas as posições da grade em um arranjo bidimensional tradicional seria extremamente
ineficiente em termos de memória, já que seria necessário reservar espaço para uma enorme
quantidade de posições nulas, além de impor um limite fixo de linhas e colunas.
Uma alternativa muito mais eficiente é utilizar uma estrutura que armazene apenas as
células cujo conteúdo é diferente de zero. Melhor ainda, essa estrutura pode ser projetada
sem que seja necessário definir previamente um número máximo de linhas ou colunas:
diferentemente de uma abordagem baseada em vetores de listas (um vetor para as linhas
e outro para as colunas), a representação cresce dinamicamente conforme novas células
são inseridas, tornando-a adequada para modelar planilhas de dimensões potencialmente
ilimitadas e esparsas.
Além disso, todo editor de planilhas moderno oferece a operação de undo (desfazer).
Uma pilha resolve esse problema de forma direta e elegante: a cada alteração, o estado
anterior da célula afetada é empilhado e, ao desfazer, o topo da pilha é consumido e o
estado anterior é restaurado.
Neste EP, implementei exatamente essa ideia: uma mini-planilha eletrônica
representada por uma matriz esparsa em listas encadeadas cruzadas (linhas e colunas não
vazias organizadas em listas ligadas, sem limite fixo de tamanho), combinada com uma
pilha dinâmica que registra o histórico de alterações e permite desfazer a última operação
realizada.
