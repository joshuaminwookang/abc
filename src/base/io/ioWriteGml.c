/**CFile****************************************************************

  FileName    [ioWriteGml.c]

  SystemName  [ABC: Logic synthesis and verification system.]

  PackageName [Command processing package.]

  Synopsis    [Procedures to write the graph structure of AIG in GML.]

  Author      [Alan Mishchenko]
  
  Affiliation [UC Berkeley]

  Date        [Ver. 1.0. Started - June 20, 2005.]

  Revision    [$Id: ioWriteGml.c,v 1.00 2005/06/20 00:00:00 alanmi Exp $]

***********************************************************************/

#include "ioAbc.h"

ABC_NAMESPACE_IMPL_START


////////////////////////////////////////////////////////////////////////
///                        DECLARATIONS                              ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
////////////////////////////////////////////////////////////////////////

/**Function*************************************************************

  Synopsis    [Writes the graph structure of AIG in GML.]

  Description [Useful for graph visualization using tools such as yEd: 
  http://www.yworks.com/]
  
  SideEffects []

  SeeAlso     []

***********************************************************************/
void Io_WriteGml( Abc_Ntk_t * pNtk, char * pFileName )
{
    FILE * pFile;
    Abc_Obj_t * pObj, * pFanin;
    int i, k;

    assert( Abc_NtkIsStrash(pNtk) || Abc_NtkIsLogic(pNtk)  );

    // start the output stream
    pFile = fopen( pFileName, "w" );
    if ( pFile == NULL )
    {
        fprintf( stdout, "Io_WriteGml(): Cannot open the output file \"%s\".\n", pFileName );
        return;
    }
    fprintf( pFile, "# GML for \"%s\" written by ABC on %s\n", pNtk->pName, Extra_TimeStamp() );
    fprintf( pFile, "graph [\n" );

    // output constant node in the AIG if it has fanouts
    if ( Abc_NtkIsStrash(pNtk) )
    {
        pObj = Abc_AigConst1( pNtk );
        if ( Abc_ObjFanoutNum(pObj) > 0 )
        {
            fprintf( pFile, "\n" );
            fprintf( pFile, "    node [ id %5d label \"%s\"\n", pObj->Id, Abc_ObjName(pObj) );
            fprintf( pFile, "        graphics [ type \"ellipse\" fill \"#CCCCFF\" ]\n" );   // grey
            fprintf( pFile, "    ]\n" );
        }
    }
    // output the POs
    fprintf( pFile, "\n" );
    Abc_NtkForEachPo( pNtk, pObj, i )
    {
        fprintf( pFile, "    node [ id %5d label \"%s\"\n", pObj->Id, Abc_ObjName(pObj) );
        fprintf( pFile, "        graphics [ type \"triangle\" fill \"#00FFFF\" ]\n" );   // blue
        fprintf( pFile, "    ]\n" );
    }
    // output the PIs
    fprintf( pFile, "\n" );
    Abc_NtkForEachPi( pNtk, pObj, i )
    {
        fprintf( pFile, "    node [ id %5d label \"%s\"\n", pObj->Id, Abc_ObjName(pObj) );
        fprintf( pFile, "        graphics [ type \"triangle\" fill \"#00FF00\" ]\n" );   // green
        fprintf( pFile, "    ]\n" );
    }
    // output the latches
    fprintf( pFile, "\n" );
    Abc_NtkForEachLatch( pNtk, pObj, i )
    {
        fprintf( pFile, "    node [ id %5d label \"%s\"\n", pObj->Id, Abc_ObjName(pObj) );
        fprintf( pFile, "        graphics [ type \"rectangle\" fill \"#FF0000\" ]\n" );   // red
        fprintf( pFile, "    ]\n" );
    }
    // output the nodes
    fprintf( pFile, "\n" );
    Abc_NtkForEachNode( pNtk, pObj, i )
    {
        fprintf( pFile, "    node [ id %5d label \"%s\"\n", pObj->Id, Abc_ObjName(pObj) );
        fprintf( pFile, "        graphics [ type \"ellipse\" fill \"#CCCCFF\" ]\n" );     // grey
        fprintf( pFile, "    ]\n" );
    }

    // output the edges
    fprintf( pFile, "\n" );
    Abc_NtkForEachObj( pNtk, pObj, i )
    {
        Abc_ObjForEachFanin( pObj, pFanin, k )
        {
            fprintf( pFile, "    edge [ source %5d   target %5d\n", pObj->Id, pFanin->Id );
            fprintf( pFile, "        graphics [ type \"line\" arrow \"first\" ]\n" );
            fprintf( pFile, "    ]\n" );
        }
    }

    fprintf( pFile, "]\n" );
    fprintf( pFile, "\n" );
    fclose( pFile );
}

/**Function*************************************************************

  Synopsis    [CUTSOM: Writes the graph structure of AIG in GML.]

  Description [Useful for graph visualization using tools such as yEd: 
  http://www.yworks.com/]
  
  SideEffects []

  SeeAlso     []

***********************************************************************/
void Io_WriteGmlNew( Abc_Ntk_t * pNtk, char * pFileName )
{
    FILE * pFile;
    Abc_Obj_t * pObj, * pFanin;
    int i, k;
    assert( Abc_NtkIsStrash(pNtk) || Abc_NtkIsLogic(pNtk)  );

    // start the output stream
    pFile = fopen( pFileName, "w" );
    if ( pFile == NULL )
    {
        fprintf( stdout, "Io_WriteGml(): Cannot open the output file \"%s\".\n", pFileName );
        return;
    }
    fprintf( pFile, "# GML for \"%s\" written by ABC on %s\n", pNtk->pName, Extra_TimeStamp() );
    fprintf( pFile, "# This is a netlist: %d , logic %d, has mapping: %d or aig %d\n",Abc_NtkIsNetlist(pNtk), Abc_NtkIsLogic(pNtk), Abc_NtkHasMapping(pNtk), Abc_NtkHasAig(pNtk));
    fprintf( pFile, "graph [\n" );

    // output constant node in the AIG if it has fanouts
    if ( Abc_NtkIsStrash(pNtk) )
    {
        pObj = Abc_AigConst1( pNtk );
        if ( Abc_ObjFanoutNum(pObj) > 0 )
        {
            fprintf( pFile, "\n" );
            fprintf( pFile, "    node [ id %5d label \"%s\"\n", pObj->Id, Abc_ObjName(pObj) );
            fprintf( pFile, "        type %d \n",  Abc_ObjType(pObj));   // type
            fprintf( pFile, "        level %d \n",  Abc_ObjLevel(pObj));   // type
            fprintf( pFile, "        fanin %d \n",  Abc_ObjFaninNum(pObj));   // sum fanin
            fprintf( pFile, "        fanout %d \n",  Abc_ObjFanoutNum(pObj));   // sum fanout
            fprintf( pFile, "    ]\n" );
        }
    }
    // output the POs
    Abc_NtkForEachPo( pNtk, pObj, i )
    {
        fprintf( pFile, "    node [ id %5d label \"%s_PO\"\n", pObj->Id, Abc_ObjName(pObj) );
        //fprintf( pFile, "        graphics [ type \"triangle\" fill \"#00FFFF\" ]\n" );   // blue
        fprintf( pFile, "        type %d \n",  Abc_ObjType(pObj));   // type
        fprintf( pFile, "        level %d \n",  Abc_ObjLevel(pObj));   // type
        fprintf( pFile, "        fanin %d \n",  Abc_ObjFaninNum(pObj));   // sum fanin
        fprintf( pFile, "        fanout %d \n",  Abc_ObjFanoutNum(pObj));   // sum fanout
        fprintf( pFile, "    ]\n" );
    }
    // output the PIs
    fprintf( pFile, "\n" );
    Abc_NtkForEachPi( pNtk, pObj, i )
    {
        fprintf( pFile, "    node [ id %5d label \"%s_PI\"\n", pObj->Id, Abc_ObjName(pObj) );
        fprintf( pFile, "        type %d \n",  Abc_ObjType(pObj));   // type
        fprintf( pFile, "        level %d \n",  Abc_ObjLevel(pObj));   // type
        fprintf( pFile, "        fanin %d \n",  Abc_ObjFaninNum(pObj));   // sum fanin
        fprintf( pFile, "        fanout %d \n",  Abc_ObjFanoutNum(pObj));   // sum fanout
        fprintf( pFile, "    ]\n" );
    }
    // output the latches
    fprintf( pFile, "\n" );
    Abc_NtkForEachLatch( pNtk, pObj, i )
    {
        fprintf( pFile, "    node [ id %5d label \"%s_Latch\"\n", pObj->Id, Abc_ObjName(pObj) );
        fprintf( pFile, "        type %d \n",  Abc_ObjType(pObj));   // type
        fprintf( pFile, "        level %d \n",  Abc_ObjLevel(pObj));   // type
        fprintf( pFile, "        fanin %d \n",  Abc_ObjFaninNum(pObj));   // sum fanin
        fprintf( pFile, "        fanout %d \n",  Abc_ObjFanoutNum(pObj));   // sum fanout
        fprintf( pFile, "    ]\n" );
    }
    // output the nodes
    fprintf( pFile, "\n" );
    Abc_NtkForEachNode( pNtk, pObj, i )
    {
        fprintf( pFile, "    node [ id %5d label \"%s_Node\"\n", pObj->Id, Abc_ObjName(pObj) );
        fprintf( pFile, "        type %d \n",  Abc_ObjType(pObj));   // type
        fprintf( pFile, "        level %d \n",  Abc_ObjLevel(pObj));   // type
        fprintf( pFile, "        fanin %d \n",  Abc_ObjFaninNum(pObj));   // sum fanin
        fprintf( pFile, "        fanout %d \n",  Abc_ObjFanoutNum(pObj));   // sum fanout
        fprintf( pFile, "    ]\n" );
    }

    // output the edges
    fprintf( pFile, "\n" );
    Abc_NtkForEachObj( pNtk, pObj, i )
    {
        for ( k = 0; (k < Abc_ObjFaninNum(pObj)) && (((pFanin) = Abc_ObjFanin(pObj, k)), 1); k++ )
        {
            fprintf( pFile, "    edge [ source %5d   target %5d\n", pObj->Id, pFanin->Id );
            if (k==0) {
                fprintf( pFile, "        invert %d \n",  pObj->fCompl0);   // inv or not
            } else {
                fprintf( pFile, "        invert %d \n",  pObj->fCompl1);   // inv or not
            }
            fprintf( pFile, "    ]\n" );
        }
    }
    fprintf( pFile, "]\n" );
    fprintf( pFile, "\n" );
    fclose( pFile );
}


////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////


ABC_NAMESPACE_IMPL_END

